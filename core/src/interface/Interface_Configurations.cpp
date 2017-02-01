#include <interface/Interface_Configurations.h>
#include <interface/Interface_State.h>
#include "Core_Defines.h"
#include <data/State.hpp>
#include <utility/Configurations.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


std::function< bool(const Vector3&, const Vector3&) > get_filter(Vector3 position, const float r_cut_rectangular[3], float r_cut_cylindrical, float r_cut_spherical, bool inverted)
{
	bool no_cut_rectangular_x = r_cut_rectangular[0] < 0;
	bool no_cut_rectangular_y = r_cut_rectangular[1] < 0;
	bool no_cut_rectangular_z = r_cut_rectangular[2] < 0;
	bool no_cut_cylindrical   = r_cut_cylindrical    < 0;
	bool no_cut_spherical     = r_cut_spherical      < 0;

	std::function< bool(const Vector3&, const Vector3&) > filter;
	if (!inverted)
	{
		filter =
			[position, r_cut_rectangular, r_cut_cylindrical, r_cut_spherical, no_cut_rectangular_x, no_cut_rectangular_y, no_cut_rectangular_z, no_cut_cylindrical, no_cut_spherical]
			(const Vector3& spin, const Vector3& spin_pos)
		{
			Vector3 r_rectangular = spin_pos - position;
			scalar r_cylindrical = std::sqrt(std::pow(spin_pos[0] - position[0], 2) + std::pow(spin_pos[1] - position[1], 2));
			scalar r_spherical   = (spin_pos-position).norm();
			if (   ( no_cut_rectangular_x || std::abs(r_rectangular[0]) < r_cut_rectangular[0] )
				&& ( no_cut_rectangular_y || std::abs(r_rectangular[1]) < r_cut_rectangular[1] )
				&& ( no_cut_rectangular_z || std::abs(r_rectangular[2]) < r_cut_rectangular[2] )
				&& ( no_cut_cylindrical   || r_cylindrical    < r_cut_cylindrical )
				&& ( no_cut_spherical     || r_spherical      < r_cut_spherical )
				) return true;
			return false;
		};
	}
	else
	{
		filter =
			[position, r_cut_rectangular, r_cut_cylindrical, r_cut_spherical, no_cut_rectangular_x, no_cut_rectangular_y, no_cut_rectangular_z, no_cut_cylindrical, no_cut_spherical]
			(const Vector3& spin, const Vector3& spin_pos)
		{
			Vector3 r_rectangular = spin_pos - position;
			scalar r_cylindrical = std::sqrt(std::pow(spin_pos[0] - position[0], 2) + std::pow(spin_pos[1] - position[1], 2));
			scalar r_spherical   = (spin_pos-position).norm();
			if (!( ( no_cut_rectangular_x || std::abs(r_rectangular[0]) < r_cut_rectangular[0] )
				&& ( no_cut_rectangular_y || std::abs(r_rectangular[1]) < r_cut_rectangular[1] )
				&& ( no_cut_rectangular_z || std::abs(r_rectangular[2]) < r_cut_rectangular[2] )
				&& ( no_cut_cylindrical   || r_cylindrical    < r_cut_cylindrical )
				&& ( no_cut_spherical     || r_spherical      < r_cut_spherical )
				)) return true;
			return false;
		};
	}

	return filter;
}

void Configuration_Domain(State *state, const float direction[3], const float position[3], const float r_cut_rectangular[3], float r_cut_cylindrical, float r_cut_spherical, bool inverted, int idx_image, int idx_chain)
{
	std::shared_ptr<Data::Spin_System> image;
	std::shared_ptr<Data::Spin_System_Chain> chain;
	from_indices(state, idx_image, idx_chain, image, chain);

	// Get relative position
	Vector3 _pos{ position[0], position[1], position[2] };
	Vector3 vpos = image->geometry->center + _pos;

	// Create position filter
	auto filter = get_filter(vpos, r_cut_rectangular, r_cut_cylindrical, r_cut_spherical, inverted);

	// Apply configuration
	Vector3 vdir{ direction[0], direction[1], direction[2] };
	Utility::Configurations::Domain(*image, vdir, filter);
}


// void Configuration_DomainWall(State *state, const float pos[3], float v[3], bool greater, int idx_image, int idx_chain)
// {
// 	std::shared_ptr<Data::Spin_System> image;
// 	std::shared_ptr<Data::Spin_System_Chain> chain;
// 	from_indices(state, idx_image, idx_chain, image, chain);

// 	// Create position filter
// 	Vector3 vpos{pos[0], pos[1], pos[2]};
// 	std::function< bool(const Vector3&, const Vector3&) > filter = [vpos](const Vector3& spin, const Vector3& position)
// 	{
// 		scalar r = std::sqrt(std::pow(position[0] - vpos[0], 2) + std::pow(position[1] - vpos[1], 2));
// 		if ( r < 3) return true;
// 		return false;
// 	};
// 	// Apply configuration
// 	Utility::Configurations::Domain(*image, Vector3{ v[0],v[1],v[2] }, filter);
// }


void Configuration_PlusZ(State *state, const float position[3], const float r_cut_rectangular[3], float r_cut_cylindrical, float r_cut_spherical, bool inverted, int idx_image, int idx_chain)
{
	std::shared_ptr<Data::Spin_System> image;
	std::shared_ptr<Data::Spin_System_Chain> chain;
	from_indices(state, idx_image, idx_chain, image, chain);

	// Get relative position
	Vector3 _pos{ position[0], position[1], position[2] };
	Vector3 vpos = image->geometry->center + _pos;

	// Create position filter
	auto filter = get_filter(vpos, r_cut_rectangular, r_cut_cylindrical, r_cut_spherical, inverted);

	// Apply configuration
	Vector3 vdir{ 0,0,1 };
	Utility::Configurations::Domain(*image, vdir, filter);
}

void Configuration_MinusZ(State *state, const float position[3], const float r_cut_rectangular[3], float r_cut_cylindrical, float r_cut_spherical, bool inverted, int idx_image, int idx_chain)
{
	std::shared_ptr<Data::Spin_System> image;
	std::shared_ptr<Data::Spin_System_Chain> chain;
	from_indices(state, idx_image, idx_chain, image, chain);

	// Get relative position
	Vector3 _pos{ position[0], position[1], position[2] };
	Vector3 vpos = image->geometry->center + _pos;

	// Create position filter
	auto filter = get_filter(vpos, r_cut_rectangular, r_cut_cylindrical, r_cut_spherical, inverted);

	// Apply configuration
	Vector3 vdir{ 0,0,-1 };
	Utility::Configurations::Domain(*image, vdir, filter);
}

void Configuration_Random(State *state, const float position[3], const float r_cut_rectangular[3], float r_cut_cylindrical, float r_cut_spherical, bool inverted, bool external, int idx_image, int idx_chain)
{
	std::shared_ptr<Data::Spin_System> image;
	std::shared_ptr<Data::Spin_System_Chain> chain;
	from_indices(state, idx_image, idx_chain, image, chain);

	// Get relative position
	Vector3 _pos{ position[0], position[1], position[2]};
	Vector3 vpos = image->geometry->center + _pos;

	// Create position filter
	auto filter = get_filter(vpos, r_cut_rectangular, r_cut_cylindrical, r_cut_spherical, inverted);

	// Apply configuration
    Utility::Configurations::Random(*image, filter, external);
}

void Configuration_Add_Noise_Temperature(State *state, float temperature, const float position[3], const float r_cut_rectangular[3], float r_cut_cylindrical, float r_cut_spherical, bool inverted, int idx_image, int idx_chain)
{
	std::shared_ptr<Data::Spin_System> image;
	std::shared_ptr<Data::Spin_System_Chain> chain;
	from_indices(state, idx_image, idx_chain, image, chain);

	// Get relative position
	Vector3 _pos{ position[0], position[1], position[2] };
	Vector3 vpos = image->geometry->center + _pos;

	// Create position filter
	auto filter = get_filter(vpos, r_cut_rectangular, r_cut_cylindrical, r_cut_spherical, inverted);

	// Apply configuration
    Utility::Configurations::Add_Noise_Temperature(*image, temperature, 0, filter);
}

void Configuration_Hopfion(State *state, float r, int order, const float position[3], const float r_cut_rectangular[3], float r_cut_cylindrical, float r_cut_spherical, bool inverted, int idx_image, int idx_chain)
{
	std::shared_ptr<Data::Spin_System> image;
	std::shared_ptr<Data::Spin_System_Chain> chain;
	from_indices(state, idx_image, idx_chain, image, chain);

	// Get relative position
	Vector3 _pos{ position[0], position[1], position[2] };
	Vector3 vpos = image->geometry->center + _pos;

	// Set cutoff radius
	if (r_cut_spherical < 0) r_cut_spherical = r*M_PI;

	// Create position filter
	auto filter = get_filter(vpos, r_cut_rectangular, r_cut_cylindrical, r_cut_spherical, inverted);

	// Apply configuration
	Utility::Configurations::Hopfion(*image, vpos, r, order, filter);
}

void Configuration_Skyrmion(State *state, float r, float order, float phase, bool upDown, bool achiral, bool rl, const float position[3], const float r_cut_rectangular[3], float r_cut_cylindrical, float r_cut_spherical, bool inverted, int idx_image, int idx_chain)
{
	std::shared_ptr<Data::Spin_System> image;
	std::shared_ptr<Data::Spin_System_Chain> chain;
	from_indices(state, idx_image, idx_chain, image, chain);

	// Get relative position
	Vector3 _pos{ position[0], position[1], position[2] };
	Vector3 vpos = image->geometry->center + _pos;

	// Set cutoff radius
	if (r_cut_cylindrical < 0) r_cut_cylindrical = r;

	// Create position filter
	auto filter = get_filter(vpos, r_cut_rectangular, r_cut_cylindrical, r_cut_spherical, inverted);

    // Apply configuration
    Utility::Configurations::Skyrmion(*image, vpos, r, order, phase, upDown, achiral, rl, false, filter);
}

void Configuration_SpinSpiral(State *state, const char * direction_type, float q[3], float axis[3], float theta, const float position[3], const float r_cut_rectangular[3], float r_cut_cylindrical, float r_cut_spherical, bool inverted, int idx_image, int idx_chain)
{
	std::shared_ptr<Data::Spin_System> image;
	std::shared_ptr<Data::Spin_System_Chain> chain;
	from_indices(state, idx_image, idx_chain, image, chain);

	// Get relative position
	Vector3 _pos{ position[0], position[1], position[2] };
	Vector3 vpos = image->geometry->center + _pos;

	// Create position filter
	auto filter = get_filter(vpos, r_cut_rectangular, r_cut_cylindrical, r_cut_spherical, inverted);

    // Apply configuration
    std::string dir_type(direction_type);
	Vector3 vq{ q[0], q[1], q[2] };
	Vector3 vaxis{ axis[0], axis[1], axis[2] };
	Utility::Configurations::SpinSpiral(*image, dir_type, vq, vaxis, theta, filter);
}