#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>

using namespace std;

// This starting point will always be the point with the smallest y-coord
point2d starter_point;

/* **************************************** */
/* Returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
int signed_area2D(point2d a, point2d b, point2d c)
{
  return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}

/* **************************************** */
/* Returns 1 if p, q, r are collinear, and 0 otherwise */
int collinear(point2d p, point2d q, point2d r)
{
  return signed_area2D(p, q, r) == 0;
}

/* **************************************** */
/* Returns 1 if c is strictly left of ab; 0 otherwise */
int left_strictly(point2d a, point2d b, point2d c)
{
  return signed_area2D(a, b, c) > 0;
}

/* Returns 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2d a, point2d b, point2d c)
{
  return signed_area2D(a, b, c) >= 0;
}

// This function compares angles of points with respect to the starter point
bool compareAngles(point2d a, point2d b)
{
  // atan2 is just a way to get the angle of a point with respect to the origin
  double angle_a = atan2(a.y - starter_point.y, a.x - starter_point.x);
  double angle_b = atan2(b.y - starter_point.y, b.x - starter_point.x);
  if (angle_a != angle_b)
    return angle_a < angle_b;
  else
  {
    // If the angles are equal then sort by their distance from the starter_point like we discussed in class to avoid a degenerate case
    int dist_a = (a.x - starter_point.x) * (a.x - starter_point.x) + (a.y - starter_point.y) * (a.y - starter_point.y);
    int dist_b = (b.x - starter_point.x) * (b.x - starter_point.x) + (b.y - starter_point.y) * (b.y - starter_point.y);
    return dist_a < dist_b;
  }
}

// Compute the convex hull of pts, and store the points on the hull in hull
void graham_scan(vector<point2d> &pts, vector<point2d> &hull)
{
  printf("hull2d (graham scan): start\n");
  hull.clear(); // Should be empty, but clear it to be safe

  // Find the starting point which is always the point with the smallest y-coord then the smallest x-coord as a tie breaker
  starter_point = pts[0];
  for (int i = 1; i < pts.size(); i++)
  {
    if (pts[i].y < starter_point.y || (pts[i].y == starter_point.y && pts[i].x < starter_point.x))
    {
      starter_point = pts[i];
    }
  }

  // Move starter_point to the first position in the vector
  for (int i = 0; i < pts.size(); i++)
  {
    if (pts[i].x == starter_point.x && pts[i].y == starter_point.y)
    {
      swap(pts[0], pts[i]);
      break;
    }
  }

  // This sorts the points using compareAngles
  sort(pts.begin() + 1, pts.end(), compareAngles);

  // Remove points that have the same angle and are further from starter_point so the degenrate case is avoided
  vector<point2d> unique_pts;
  unique_pts.push_back(pts[0]);
  for (int i = 1; i < pts.size(); ++i)
  {
    while (i < pts.size() - 1 && collinear(starter_point, pts[i], pts[i + 1]))
      i++;
    unique_pts.push_back(pts[i]);
  }
  // Replace pts with unique_pts
  pts = unique_pts;

  // Check if we have at least 3 points
  if (pts.size() < 3)
  {
    hull = pts;
    printf("hull2d (graham scan): end\n");
    return;
  }

  // Initialize the hull with the first three points
  hull.push_back(pts[0]);
  hull.push_back(pts[1]);
  hull.push_back(pts[2]);

  // Iterate through the rest of the points
  for (int i = 3; i < pts.size(); i++)
  {
    // Pop points off the hull until we find a left turn
    while (hull.size() >= 2 && signed_area2D(hull[hull.size() - 2], hull[hull.size() - 1], pts[i]) <= 0)
    {
      // Pop the last point off
      hull.pop_back();
    }
    // Push the current point onto the hull
    hull.push_back(pts[i]);
  }

  printf("hull2d (graham scan): end\n");
  printf("Here are the vertices in the hull\n");
  for (int i = 0; i < hull.size(); i++)
  {
    printf("(%d, %d)\n", hull[i].x, hull[i].y);
  }

  return;
}
