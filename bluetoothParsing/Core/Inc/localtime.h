#ifndef __localtime_H
#define __localtime_H

#include <time.h>


#define year_position  					(	int	)			0
#define month_position  				(	int	)			1
#define day_position  					(	int	)			2
	
#define hour_position  					(	int	)			3
#define minutes_position  			(	int	)			4
#define seconds_position  			(	int	)			5

typedef struct localtime
{
	struct tm* localtim;
	unsigned int updated;
	unsigned int need_update;
	unsigned int fingerp;
} localtime1;

extern localtime1 stmtime;
extern localtime1 aux;
extern localtime1 aux1;

#endif
