#include "PID.h"

using namespace std;

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd)
{
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    this->p_error = 0;
    this->i_error = 0;
    this->d_error = 0;
    this->had_first_data = false;
}

void PID::UpdateError(double cte)
{
    if (had_first_data)
    {
        double prev_cte = p_error;
        p_error = cte;
        i_error += cte;
        d_error = cte - prev_cte;
    }
    else
    {
        p_error = cte;
        i_error = cte;
        d_error = 0;
        had_first_data = true;
    }
}

double PID::TotalError()
{
    return Kp * p_error + Ki * i_error + Kd * d_error;
}
