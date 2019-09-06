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

class metropolis_hastings {
    public:
        metropolis_hastings(std::vector<double> &params, std::vector<double> d):
            data(d),                //import data
            dist(0.0,1.0),
            transition(0.0,1.0),
            theta(params),          //get model parameters
            numParams(theta.size())
        {
            theta_new.resize(numParams, 0);
            stepCount = 0;
            generator = std::mt19937(rd());
        }

        virtual double log_likelihood(std::vector<double> &x) = 0;
        virtual double log_prior(std::vector<double> &x) = 0;
        
        //Transition model, to sample for new random parameters
        void transition_normal(){
            for(int i=0; i<numParams; i++)
                theta_new[i] = theta[i] + transition(generator); 
        }
    
        void run_model(){
            for(int i=1; i<iterations; ++i){
                transition_normal(); //sets theta_new
                double theta_lik = log_likelihood(theta);
                double theta_new_lik = log_likelihood(theta_new);
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

    protected:
        int iterations = 40000;
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

};

class MH_normal: public metropolis_hastings {
    public:
        MH_normal(std::vector<double> &params, std::vector<double> d):
            metropolis_hastings(params, d){ }
    
        double log_prior(std::vector<double> &x){
            for(int i=0; i<numParams; i++)
                if (x[i] <= 0)
                    return std::log(0);
            return std::log(1);
        }

        //likelihood computer
        double log_likelihood(std::vector<double> &x){
            int N = data.size();
            double likelihood = -N*std::log(x[1]*std::sqrt(2*M_PI));
            for(int i=0; i<N; ++i){
                likelihood -= (std::pow(data[i]-x[0], 2))/(2*pow(x[1],2));
            }
            return likelihood;
        }
};


