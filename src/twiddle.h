#ifndef TWIDDLE_H
#define TWIDDLE_H
class PID;

class Twiddle
{
    double tolerance_;
    int samples_;
    double dp_[3];
    double error_;
    double best_error_;
    int i_;
    int next_label_;
    int step_;

  public:
    double p_[3];
    Twiddle(double p0, double p1, double p2, double tolerance, int samples);
    virtual ~Twiddle();
    void Step(double cte);
    double SumDp();
    void Print();
};

#endif /* TWIDDLE_H */
