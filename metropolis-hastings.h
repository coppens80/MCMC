#include <stdio.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>

/* template <class Type> class norm_params{ */
/*     public: */
/*         Type mu, sigma; */
/* }; */

class MH_model {
    //Data
    private:
        int iterations = 20000;
        int stepCount;
        std::vector<double> theta; //model parameters
        std::vector<double> theta_new; //model parameters
        int numParams;
        std::mt19937 generator;
        std::random_device rd;
        std::uniform_real_distribution<double> dist;
        std::normal_distribution<double> transition;
        std::vector<std::vector<double>> accepted;
        std::vector<std::vector<double>> rejected;
        std::vector<int> Aindices, Rindices;
        std::vector<double> data;
        std::ofstream acceptions, rejections;

    //Methods
    public:
        //Constructor
        MH_model(std::vector<double> &params, std::vector<double> d):
            data(d),                //import data
            dist(0.0,1.0),
            transition(0.0,1.0),
            theta(params),          //get model parameters
            numParams(theta.size())
        {
            theta_new.resize(numParams, 0);
        }

        void Initialize(){
            stepCount = 0;
            generator = std::mt19937(rd());
            run_model();
        }
        void run_model(){
            // likelihood_computer(x,data): returns the likelihood that these parameters generated the data
            // transition_model(x): a function that draws a sample from a symmetric distribution and returns it
            // param_init: a starting sample
            // iterations: number of accepted to generated
            // data: the data that we wish to model
            // acceptance_rule(x,x_new): decides whether to accept or reject the new sample
            for(int i=1; i<iterations; ++i){
                transition_normal(); //sets theta_new
                double theta_lik = log_lik_normal(theta);
                double theta_new_lik = log_lik_normal(theta_new);
                if(acceptance(theta_lik + log_prior(theta), theta_new_lik + log_prior(theta_new))){
                    theta = theta_new;
                    accepted.push_back(theta_new);
                    Aindices.push_back(i);
                }else{
                    rejected.push_back(theta_new);
                    Rindices.push_back(i);
                }
            }
            SaveModelData();
        }

    private:
        //Transition model, to sample for new random parameters
        void transition_normal(){
            for(int i=0; i<numParams; i++)
                theta_new[i] = theta[i] + transition(generator); 
        }

        //Prior probability
        //returns Log(1) for all valid values of sigma. Log(1) =0, so it does not affect the summation.
        //returns Log(0) for all invalid parameter (<=0). Log(0)=-infinity, and Log(negative number) is undefined.
        //It makes the new sigma infinitely unlikely.
        double log_prior(std::vector<double> &x){
            for(int i=0; i<numParams; i++)
                if (x[i] <= 0)
                    return std::log(0);
            return std::log(1);
        }

        //likelihood computer
        double log_lik_normal(std::vector<double> &x){
            int N = data.size();
            //int N = datasize
            double likelihood = -N*std::log(x[1]*std::sqrt(2*M_PI));
            for(int i=0; i<N; ++i){
                likelihood -= (std::pow(data[i]-x[0], 2))/(2*pow(x[1],2));
            }
            return likelihood;
        }

        //acceptance rule
        int acceptance(double x_prev, double x_new){
            if (x_new > x_prev)
                return 1;
            else{
                double r = dist(generator); //get random uniform int from 0 to 1
                return (r < std::exp(x_new-x_prev));
            }
        }
        
        void SaveModelData(){
            acceptions.open("DATA/accepted.csv");
            for (int i=0; i<accepted.size(); i++) {
                acceptions << Aindices[i] << "," << accepted[i][0] << "," << accepted[i][1] << std::endl;
            }
            acceptions.close();

            rejections.open("DATA/rejected.csv");
            for (int i=0; i<rejected.size(); i++) {
                rejections << Rindices[i] << "," << rejected[i][0] << "," << rejected[i][1] << std::endl;
            }
            rejections.close();
        }
};
