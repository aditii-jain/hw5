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
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<int>& shiftCount,
    int day,
    int slot
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
) {
    if (avail.empty() || avail[0].empty()) return false;

    int days = avail.size();
    int workers = avail[0].size();

    sched.clear();
    sched.resize(days);  // resize to hold a vector per day
    std::vector<int> shiftCount(workers, 0); // shift counter for each worker

    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftCount, 0, 0);
}


bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<int>& shiftCount,
    int day,
    int slot
) {
    if (day == avail.size()) return true; // all days filled

    int numWorkers = avail[0].size();

    // if current day is filled, move to next day
    if (slot == dailyNeed) {
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftCount, day + 1, 0);
    }

    for (int worker = 0; worker < numWorkers; ++worker) {
        if (avail[day][worker] && shiftCount[worker] < maxShifts) {
            // check if already assigned today
            if (std::find(sched[day].begin(), sched[day].end(), worker) != sched[day].end()) continue;

            sched[day].push_back(worker);
            shiftCount[worker]++;

            if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftCount, day, slot + 1)) {
                return true;
            }

            // backtrack
            sched[day].pop_back();
            shiftCount[worker]--;
        }
    }

    return false;
}