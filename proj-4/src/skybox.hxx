#include "cs237.hxx"
/* The path to the shaders; this is usually set from the compiler command-line.
 * but it defaults to a path relative to the build directory.
 */


#ifndef DATA_DIR
#  define DATA_DIR "../data"
#endif

struct Wall;

/*! \brief the sides of the cube. */
enum Side {
    LEFT = 0,		/*!< left side:   x == -1 */
    RIGHT = 1,		/*!< right side:  x == +1 */
    BOTTOM = 2,		/*!< bottom side: y == -1 */
    TOP = 3,		/*!< top side:    y == +1 */
    FRONT = 4,		/*!< front side:  z == -1 */
    REAR = 5,		/*!< rear side:   z == +1 */
    NUM_SIDES = 6		/*!< the number of sides */
};
void InitBox(struct View *view);
void ScaleWall(struct View *view);

void DrawWall (struct View *view, int w);


