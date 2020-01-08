#ifndef TIMEF_H
#define TIMEF_H

enum Month { January = 0, February, March, April, May, June, July, August, September,October, Movember, December };

enum DayEndMonth { mJanuary   =               31,       mFebruary       = mJanuary + 28,      mMarch          = mFebruary + 31,

                   mApril     = mMarch      + 30,       mMay            = mApril   + 31,      mJune           = mMay      + 30,

                   mJuly      = mJune       + 31,       mAugust         = mJuly    + 31,      mSeptember      = mAugust   + 30,

                   mOctober   = mSeptember  + 31,   mMovember           = mOctober + 30,      mDecember       = mOctober  + 31 };

int nDayEndMonth [ ] = { mJanuary, mFebruary, mMarch, mApril, mMay, mJune, mJuly, mAugust, mSeptember, mOctober, mMovember, mDecember };

int numberDaysuntil ( int day, int month )
{
  return nDayEndMonth[month] + day;
}


#endif // TIMEF_H
