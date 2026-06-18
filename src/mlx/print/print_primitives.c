#include "scene.h"

//	Static Functions
static void	print_plane(t_objs pl);
static void	print_sphere(t_objs sp);
static void	print_cylinder(t_objs cy);

void	geometric_primitives(t_objs *obj)
{
	static void	(*print_primitives[])(t_objs) = {
		print_plane,
		print_sphere,
		print_cylinder
	};
	print_primitives[obj->type](*obj);
}

static void	print_plane(t_objs pl)
{
	printf("\nPlane\nCoordinates: (x=%f, y=%f, z=%f)\n  "
		"Direction: (%f, %f, %f)\n     "
		"Colour: (%d, %d, %d)\n",
		pl.coords[X], pl.coords[Y], pl.coords[Z],
		pl.u.plane.orientation[X], pl.u.plane.orientation[Y], pl.u.plane.orientation[Z],
		(int)(pl.color[R] * 255), (int)(pl.color[G] * 255),
		(int)(pl.color[B] * 255));
}

static void	print_sphere(t_objs sp)
{
	printf("\nSphere\nCoordinates: (x=%f, y=%f, z=%f)\n   "
		"Diameter: %f\n     Radius: %f\n     Colour: (%d, %d, %d)\n",
		sp.coords[X], sp.coords[Y], sp.coords[Z],
		sp.u.sphere.radius * 2, sp.u.sphere.radius,
		(int)(sp.color[R] * 255), (int)(sp.color[G] * 255),
		(int)(sp.color[B] * 255));
}

static void	print_cylinder(t_objs cy)
{
	const t_cylinder	c = cy.u.cylinder;

	printf("\nCylinder\nCoordinates: (x=%f, y=%f, z=%f)\n  Direction: (%f, %f, %f)\n"
		"Diameter: %f\n     Radius: %f\n     "
		"Height: %f\n     Colour: (%d, %d, %d)\n",
		cy.coords[X], cy.coords[Y], cy.coords[Z],
		c.orientation[X], c.orientation[Y], c.orientation[Z],
		c.radius * 2, c.radius, c.height,
		(int)(cy.color[R] * 255), (int)(cy.color[G] * 255),
		(int)(cy.color[B] * 255));
}
