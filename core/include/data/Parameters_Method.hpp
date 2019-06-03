#pragma once
#ifndef DATA_PARAMETERS_METHOD_H
#define DATA_PARAMETERS_METHOD_H

#include "Spirit_Defines.h"
#include <io/Fileformat.hpp>

#include <string>

namespace Data
{
    // Solver Parameters Base Class
    struct Parameters_Method
    {
        // --------------- Iterations ------------
        // Number of iterations carried out when pressing "play" or calling "iterate"
        long int n_iterations = 1e6;
        // Number of iterations after which the Method should save data
        long int n_iterations_log = 1e3;

        // Maximum walltime for Iterate in seconds
        long int max_walltime_sec = 0;

        // Torque convergence criterium
        scalar torque_convergence = 1e-10;

        // ----------------- Output --------------
        // Data output folder
        std::string output_folder = "output";
        // Put a tag in front of output files (if "<time>" is used then the tag is the timestamp)
        std::string output_file_tag = "<time>";
        // Save any output when logging
        bool output_any = false;
        // Save output at initial state
        bool output_initial = false;
        // Save output at final state
        bool output_final = false;
        // Vectorfield output file format
        IO::VF_FileFormat output_vf_filetype = IO::VF_FileFormat::OVF_TEXT;
    };
}
#endif