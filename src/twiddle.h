#ifndef TWIDDLE_H
#define TWIDDLE_H
class PID;

class Twiddle
{
    PID *const pid_;
    double tolerance_;
    int samples_;
    double p_[3];
    double dp_[3];
    double error_;
    double best_error_;
    int i_;
    int next_label_;
    int step_;

  public:
    Twiddle(PID* pid, double tolerance, int samples);
    virtual ~Twiddle();
    void Step(double cte);
    double SumDp();
    void SetPidParams();
    void Print();
};

#endif /* TWIDDLE_H */
