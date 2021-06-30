/*
The explanation and evaluation of this code is given in the SMA Tenkan-sen Kijun-sen model document also in the repository.

For questions or comments you can contact my reddit account: s1123581321
*/

#include <iostream>
#include <fstream>

using namespace std;

//Indicator calculation functions
void Ts_func(float *Ts,float *close,int pnt);   //Tenkan-sen
void Ks_func(float *Ks,float *close,int pnt);   //Kijun-sen
void SMA_func(float *SMA,float *close,int SMA_period,int pnt);    //SMA

int main(){

    //Reading the data out of the file
    //Data from https://www.histdata.com/ in the "Generic ASCII" format
    ifstream data_file("data.csv");
    string entry;
    int data_file_lines=0;

    //Finds the total number of lines in the file
    while(getline(data_file,entry)){
        data_file_lines=data_file_lines+1;
    }
    data_file.clear();
    data_file.seekg(0);

    //Variables to read the file into
    float close[data_file_lines];
    float full;
    char semi;
    int i=0;

    //Reads the data out of the file, the only data used in analysis is the close price
    while(!data_file.eof()){
        data_file >> full >> full >> semi >> full >> semi >> full >> semi >> full >> semi >> close[i] >> semi >> full;
        i=i+1;
    }

    data_file.close();

    //Now have full array of close prices



    //Indicator variables
    float Ts=0,Ks=0,SMA=0;
    int SMA_period=4;   //Variable for the number of periods that the SMA should be calculated for
    bool b_initial=false,b_con1=false,b_con2=false,b_open=false;    //Conditions
    float b_open_price=0,s_open_price=0,pl=0;
    bool s_initial=false,s_con1=false,s_con2=false,s_open=false;


    //Opening a file to write the close prices, indicator values and all types of profit/loss information
    ofstream output_file("output.txt");
    ofstream pl_output_file("pl_output.txt");

    for(int pnt=0;pnt<data_file_lines;++pnt){

        //Calculating indicator values
        Ts_func(&Ts,close,pnt);
        Ks_func(&Ks,close,pnt);
        SMA_func(&SMA,close,SMA_period,pnt);
        output_file<<close[pnt]<<" "<<Ts<<" "<<Ks<<" "<<SMA<<endl;

        if(Ts!=0&&Ks!=0&&SMA!=0){

            //Buy orders
            if(b_open==false){
                if(Ks<Ts&&Ts<SMA&&b_con2==true){
                    b_con2=false;
                    b_open=true;
                    b_open_price=close[pnt];
                }
                if(Ts<Ks&&Ks<SMA&&b_con1==true){
                    b_con1=false;
                    b_con2=true;
                }
                if(Ts<SMA&&SMA<Ks&&b_initial==true){
                    b_initial=false;
                    b_con1=true;
                }
                if(SMA<Ts&&Ts<Ks){
                    b_initial=true;
                }
            }else if(b_open==true){
                if(Ks>Ts){
                    b_open=false;
                    //pl=close[pnt]-b_open_price;
                    pl=b_open_price-close[pnt];
                    pl_output_file<<pl<<endl;
                }
            }

            //Sell orders
            if(s_open==false){
                if(Ks>Ts&&Ts>SMA&&s_con2==true){
                    s_con2=false;
                    s_open=true;
                    s_open_price=close[pnt];
                }
                if(Ts>Ks&&Ks>SMA&&s_con1==true){
                    s_con1=false;
                    s_con2=true;
                }
                if(Ts>SMA&&SMA>Ks&&s_initial==true){
                    s_initial=false;
                    s_con1=true;
                }
                if(SMA>Ts&&Ts>Ks){
                    s_initial=true;
                }
            }else if(s_open==true){
                if(Ks<Ts){
                    s_open=false;
                    //pl=s_open_price-close[pnt];
                    pl=close[pnt]-s_open_price;
                    pl_output_file<<pl<<endl;
                }
            }

        }
    }

    output_file.close();
    pl_output_file.close();

    return 0;

}

void Ts_func(float *Ts,float *close,int pnt){
    float cmax=0,cmin=3;
    *Ts=0;
    if(pnt>7){
        for(int j=pnt-8;j<pnt+1;++j){
            if(close[j]>cmax){
                cmax=close[j];
            }
            if(close[j]<cmin){
                cmin=close[j];
            }
        }
        *Ts=(cmax+cmin)/2;
    }
}

void Ks_func(float *Ks,float *close,int pnt){
    float cmax=0,cmin=3;
    *Ks=0;
    if(pnt>24){
        for(int j=pnt-25;j<pnt+1;++j){
            if(close[j]>cmax){
                cmax=close[j];
            }
            if(close[j]<cmin){
                cmin=close[j];
            }
        }
        *Ks=(cmax+cmin)/2;
    }
}

void SMA_func(float *SMA,float *close,int SMA_period,int pnt){
    if(pnt+1>=SMA_period){
        *SMA=0;
        for(int i=pnt-SMA_period+1;i<pnt+1;++i){
            *SMA=close[i]+*SMA;
        }
        *SMA=*SMA/SMA_period;
    }
}
