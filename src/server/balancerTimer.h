#ifndef BALANCERTIMER_H
#define BALANCERTIMER_H

#include <chrono>
#include <ctime>
#include <queue>

typedef std::chrono::duration<double> duration;
typedef std::chrono::high_resolution_clock hr_time;

class BalancerTimer
{
private:
  std::queue<hr_time::time_point> sender_times;

public:
  BalancerTimer() {}
  duration measureTimeSinceLastSend()
  {
    return (hr_time::now() - sender_times.front());
  }
  bool exceedsNumSendingLimit(const int &lim) { return (sender_times.size() < lim); }
  bool checkDurationPerLastSecond(duration period)
  {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(period).count() >= 1000)
    {
      sender_times.pop();
      return true;
    }
    else
    {
      return false;
    }
  }
  void addCurrentSenderTime() { sender_times.push(hr_time::now()); }
  bool allowSendingData(const unsigned long int &lim)
  {
    if (exceedsNumSendingLimit(lim))
    {
      return true;
    }
    else if (!sender_times.empty())
    {
      return checkDurationPerLastSecond(measureTimeSinceLastSend());
    }
    else
    {
      return true;
    }
  };

  ~BalancerTimer() {}
};

#endif // BALANCERTIMER_H