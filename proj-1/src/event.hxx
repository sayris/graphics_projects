/*! \file event.hxx
 *
 * \brief the representation of events in the simulation.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _EVENT_HXX_
#define _EVENT_HXX_

#include "cs237.hxx"
#include "ball.hxx"
#include "wall.hxx"

//! \brief the generic representation of simulation events
class Event {
  public:
    double at() const		{ return this->_at; }

    virtual ~Event () = 0;

  protected:
    double		_at;		//!< the time at which the event occurs

    Event (double t);

    virtual bool InvolvesBall (const Ball *bb) = 0;
    virtual void Cancel (const Ball *b) = 0;
    virtual void DoEvent () = 0;

    static Event	*PQ[MAX_NBALLS+1];	//!< \brief the priority queue of simulation events
    static int		NEvents;		//!< \brief the number of pending events in #PQ[]

    static void Insert (Event *evt);
    static Event *Next (double t);
    static Event *Find (Ball *ball);

  friend void PredictEvent (Ball *b);
  friend void Simulate (double now);
  friend void CancelEvent (Ball *b);
};

/***** Event operations *****/

//! \brief determine the next event that will involve the ball
//! \param b ball to predict events for
//
void PredictEvent (Ball *b);

//! \brief smulate an events that occur at or before the given time
//! \param now the current simulation time
//
void Simulate (double now);

//! \brief cancel the event associated with the given ball
//! \param b the ball that is involved in the event.
//
//! Cancel the given event, which means freeing its storage and, if it is a
//! ball-ball collision, rescheduling the other ball.
//
void CancelEvent (Ball *b);

#endif /* !_EVENT_HXX_ */
