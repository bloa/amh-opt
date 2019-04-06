/*
  Copyright (C) ORKAD team, CRIStAL, University of Lille, 2017
  Aymeric Blot

  This software is governed by the CeCILL-C license.
  You can use, modify and/ or redistribute the software under the
  terms of the CeCILL-C license as circulated by CEA, CNRS and INRIA
  at the following URL "http://www.cecill.info".
*/

// general stuff
#include <amh.h>

// termination criteria
#include <opt/check_chrono.h>
#include <opt/check_ctrlc.h>
#include <opt/check_iter.h>
#include <opt/check_stag.h>
#include <opt/check_time.h>

// representation & fitness
#include <opt/fitness_base.h>
#include <opt/fitness_min.h>
#include <opt/fitness_max.h>
#include <opt/eval_perm.h>
#include <opt/base_sol.h>
#include <opt/permutation.h>
#include <opt/permutation_obj.h>
#include <opt/archive.h>

// neighborhoods
#include <opt/nbh_move.h>
#include <opt/nbh_swap.h>
#include <opt/nbh_adjswap.h>
#include <opt/nbh_shift.h>
#include <opt/nbh_shiftmswap.h>

// stuff
#include <opt/gen_shuffle.h>
#include <opt/agreg.h>
#include <opt/agreg_normal.h>
#include <opt/pareto.h>
#include <opt/pareto_dmls.h>
