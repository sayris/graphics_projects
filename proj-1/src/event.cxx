/*! \file event.c
 *
 * \brief the event-driven simulation
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "wall.hxx"
#include "ball.hxx"
#include "event.hxx"

Event	*Event::PQ[MAX_NBALLS+1];	//!< \brief the priority queue of simulation events
int	Event::NEvents;			//!< \brief the number of pending events in #PQ[]


//! \brief the representation of ball-wall collision events
class WallEvent : public Event {
  public:

    WallEvent (double at, Ball *b, Side s);
    ~WallEvent ();

    bool InvolvesBall (const Ball *bb);
    void Cancel (const Ball *b);
    void DoEvent ();

  protected:
    Ball	*_b;		//!< the ball involved in the collision
    Side	_side;		//!< the side of the cube being collided with
};

//! \brief the representation of ball-ball collision events
class BallEvent : public Event {
  public:
    BallEvent (double at, Ball *b1, Ball *b2);
    ~BallEvent ();

    bool InvolvesBall (const Ball *bb);
    void Cancel (const Ball *b);
    void DoEvent ();

  protected:
    Ball	*_b1;		//!< one of the balls involved in the collision
    Ball	*_b2;		//!< the other ball involved in the collision
};


/******************** base class Event member functions ********************/

Event::Event (double t) : _at(t) { }
Event::~Event () { }

//! \brief insert an event into the priority queue
//! \param evt the event to be inserted.
//
void Event::Insert (Event *evt)
{
    int		i;
    double	t = evt->_at;

    for (i = 0;  i < Event::NEvents;  i++) {
	if (t < Event::PQ[i]->_at)
	    break;
    }
    for (int j = Event::NEvents;  j > i;  j--) {
	Event::PQ[j] = Event::PQ[j-1];
    }
    Event::PQ[i] = evt;
    Event::NEvents++;

} /* end of Event::Insert */

//! \brief get the next event from the priority queue that occurs at or before
//!   a given time.
//! \param t a time value
//! \return the next event that occurs at or before \a t.  If no such event
//!   exists, then 0 is returned.
//
Event *Event::Next (double t)
{
    int		i;

    if ((Event::NEvents > 0) && (Event::PQ[0]->_at <= t)) {
	Event *evt = Event::PQ[0];
	for (i = 1;  i < NEvents;  i++) {
	    Event::PQ[i-1] = Event::PQ[i];
	}
	Event::NEvents--;
	return evt;
    }
    else
	return nullptr;

} /* end of Event::Next */

//! \brief find the event that involves the given ball and remove it from the queue.
//! \param ball the ball that is involved in the event.
//
Event *Event::Find (Ball *ball)
{
    int		i;
    Event	*evt;

  /* search the queue for the event, setting evt to it */
    for (i = 0;  i < Event::NEvents;  i++) {
	evt = Event::PQ[i];
	if (evt->InvolvesBall(ball))
	    break;
    }

  /* we shouldn't get here, since there should always be an event for a ball */
    assert (i < Event::NEvents);

  // slide the queue entries that follow evt down one slot.
    Event::NEvents--;
    for (; i < Event::NEvents;  i++) {
	Event::PQ[i] = Event::PQ[i+1];
    }

    return evt;

} /* end of Event::Find */

/******************** class WallEvent member functions ********************/

/*! \brief allocate and initialize a new wall event
 *  \param at the time at which the event occurs
 *  \param b the ball
 *  \param s the side of the cube that the ball hits
 *  \return the new event.
 */
WallEvent::WallEvent (double at, Ball *b, Side s)
    : Event(at), _b(b), _side(s)
{ }

WallEvent::~WallEvent ()
{ }

bool WallEvent::InvolvesBall (const Ball *b)
{
    return (this->_b == b);
}

void WallEvent::Cancel (const Ball *b)
{
    /* nothing to do */
}

//! \brief process a wall event
//! \param evt the wall event to be processed
//
void WallEvent::DoEvent ()
{
  /* compute the new position and direction vector */
    WallBounce (this->_b, this->_side);

  /* determine the next event for this ball */
    PredictEvent (this->_b);

} /* end of WallEvent::DoEvent */

/******************** class BallEvent member functions ********************/

/*! \brief allocate and initialize a new ball event
 *  \param at the time at which the event occurs
 *  \param b1 the first ball
 *  \param b2 the second ball
 *  \return the new event.
 */
BallEvent::BallEvent (double at, Ball *b1, Ball *b2)
    : Event(at), _b1(b1), _b2(b2)
{ }

BallEvent::~BallEvent ()
{ }

bool BallEvent::InvolvesBall (const Ball *b)
{
    return (this->_b1 == b) || (this->_b2 == b);
}

void BallEvent::Cancel (const Ball *b)
{
  // we need to recompute the event for the other ball
    Ball *otherBall = (this->_b1 == b ? this->_b2 : this->_b1);
    PredictEvent (otherBall);
}

//! \brief process a ball event
//! \param evt the wall event to be processed
//
void BallEvent::DoEvent ()
{
  /* compute the new directions for the balls */
    BallBounce (this->_b1, this->_b2);

  /* determine the next event for the balls */
    PredictEvent (this->_b1);
    PredictEvent (this->_b2);

} /* end of BallEvent::DoEvent */

/******************** other functions ********************/

/* CancelEvent:
 */
void CancelEvent (Ball *b)
{
    Event *evt = Event::Find (b);
    assert (evt != nullptr);
    evt->Cancel(b);
    delete evt;

}

//! \brief determine the next event that will involve the ball
//! \param b ball to predict events for
//
void PredictEvent (Ball *b)
{
    double	wallHitT, ballHitT;
    Side	side;

    if (b == nullptr)
	return;

  // determine the next wall collision for the ball
    HitTest (b->pos, side, wallHitT);
    wallHitT += b->initT;

  // check for ball-ball collisions
    Ball *otherBall = nullptr;
    double minHitT = wallHitT;
    for (int i = 0;  i < Ball::NumBalls;  i++) {
	if (Ball::Balls[i] == b) continue;
	if (Collide(b, Ball::Balls[i], ballHitT)
	&& (ballHitT < minHitT)
	&& (ballHitT < Ball::Balls[i]->hitT)) {
	    otherBall = Ball::Balls[i];
	    minHitT = ballHitT;
	}
    }

    if (otherBall == 0) {
      // no ball-ball collisions, so record the wall hit
	b->hitT = wallHitT;
	Event::Insert (new WallEvent (b->hitT, b, side));
    }
    else {
      // a ball-ball collision
	b->hitT = minHitT;
	otherBall->hitT = b->hitT;
	CancelEvent (otherBall);
	Event::Insert (new BallEvent (b->hitT, b, otherBall));
    }

}

//! \brief smulate an events that occur at or before the given time
//! \param now the current simulation time
//
void Simulate (double now)
{
    Event	*evt;

    while ((evt = Event::Next(now)) != nullptr) {
	evt->DoEvent();
	delete evt;
    }

} /* end of Simulate */
