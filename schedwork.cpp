#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, vector<size_t>& shifts, size_t day, size_t slot);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U)
    {
        return false;
    }
    //sched.clear();
    // Add your code below
    size_t n = avail.size();//# of days
    size_t k = avail[0].size(); //# of workers
    for(size_t d = 0; d < n; ++d)
    {
      size_t availableCount = 0;
      for(size_t w = 0; w < k; ++w)
      {
        if(avail[d][w])
        {
          availableCount++;
        }
      }
      if(availableCount < dailyNeed)
      {
        return false; 
      }
    }
    sched.clear();
    sched.resize(n, vector<Worker_T>(dailyNeed, 0));
    vector<size_t>shifts(k,0);
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, 0, 0);
}

bool scheduleHelper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, vector<size_t>& shifts, size_t day, size_t slot)
{
  size_t n = avail.size();//# of days
  size_t k = avail[0].size(); //# of workers
  if(day == n)
  {
    return true;
  }
  if(slot == dailyNeed)
  {
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, day+1, 0);
  }
  for(Worker_T w = 0; w < k; ++w)
  {
    if(avail[day][w] && shifts[w] < maxShifts)
    {
      auto it = std::find(sched[day].begin(), sched[day].begin() + slot, w);
      if(it == sched[day].begin() + slot)
      {
        sched[day][slot] = w;
        shifts[w]++;
        if(scheduleHelper(avail, dailyNeed, maxShifts, sched, shifts, day, slot + 1))
        {
          return true;
        }
        //backtrack
        shifts[w]--;
        sched[day][slot] = 0;
      }
    }
  }
  return false;
}
