#include "twiddle.h"

#include <iostream>
#include <cassert>
#include "PID.h"

Twiddle::Twiddle(PID *pid, double tolerance, int samples)
    : pid_(pid),
      tolerance_(tolerance),
      samples_(samples),
      p_{pid->Kp, pid->Ki, pid->Kd},
      dp_{pid->Kp / 10, pid->Ki / 10, pid->Kd / 10},
      error_(0),
      best_error_(0),
      i_(0),
      next_label_(0),
      step_(-1)
{
}

Twiddle::~Twiddle() {}

void Twiddle::Step(double cte)
{
    step_ = (step_ + 1) % samples_;
    if (step_ >= 20)
    {
        error_ += cte * cte;
    }
    if (step_ == 0)
    {
        switch (next_label_)
        {
        case 0:
            goto l0;
        case 1:
            goto l1;
        case 2:
            goto l2;
        case 3:
            goto l3;
        default:
            assert(false);
        }
    }
    return;
l0:
    error_ = cte * cte;
    next_label_ = 1;
    return;
l1:
    best_error_ = error_;
    Print();
    while (SumDp() > tolerance_)
    {
        for (i_ = 0; i_ < 3; i_++)
        {
            p_[i_] += dp_[i_];
            SetPidParams();
            error_ = cte * cte;
            next_label_ = 2;
            return;
        l2:
            Print();
            if (error_ < best_error_)
            {
                best_error_ = error_;
                dp_[i_] *= 1.1;
            }
            else
            {
                p_[i_] -= 2.0 * dp_[i_];
                SetPidParams();
                error_ = cte * cte;
                next_label_ = 3;
                return;
            l3:
                Print();
                if (error_ < best_error_)
                {
                    best_error_ = error_;
                    dp_[i_] *= 1.1;
                }
                else
                {
                    p_[i_] += dp_[i_];
                    SetPidParams();
                    dp_[i_] *= 0.9;
                }
            }
        }
    }
    std::exit(0);
}

double Twiddle::SumDp()
{
    return dp_[0] + dp_[1] + dp_[2];
}

void Twiddle::SetPidParams()
{
    pid_->Kp = p_[0];
    pid_->Ki = p_[1];
    pid_->Kd = p_[2];
    pid_->i_error = 0;
}

void Twiddle::Print()
{
    std::cout << "p:" << p_[0] << " " << p_[1] << " " << p_[2] << std::endl;
    std::cout << "dp:" << dp_[0] << " " << dp_[1] << " " << dp_[2] << std::endl;
    std::cout << "error:" << error_ << std::endl;
    std::cout << "best error:" << std::min(error_, best_error_) << std::endl;
    std::cout << std::endl;
}