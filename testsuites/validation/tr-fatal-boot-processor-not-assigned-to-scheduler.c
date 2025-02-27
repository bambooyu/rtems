/* SPDX-License-Identifier: BSD-2-Clause */

/**
 * @file
 *
 * @ingroup ScoreSmpValFatalBootProcessorNotAssignedToScheduler
 */

/*
 * Copyright (C) 2021 embedded brains GmbH (http://www.embedded-brains.de)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This file is part of the RTEMS quality process and was automatically
 * generated.  If you find something that needs to be fixed or
 * worded better please post a report or patch to an RTEMS mailing list
 * or raise a bug report:
 *
 * https://www.rtems.org/bugs.html
 *
 * For information on updating and regenerating please refer to the How-To
 * section in the Software Requirements Engineering chapter of the
 * RTEMS Software Engineering manual.  The manual is provided as a part of
 * a release.  For development sources please refer to the online
 * documentation at:
 *
 * https://docs.rtems.org
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/smpimpl.h>

#include "tr-fatal-boot-processor-not-assigned-to-scheduler.h"

#include <rtems/test.h>

/**
 * @defgroup ScoreSmpValFatalBootProcessorNotAssignedToScheduler \
 *   spec:/score/smp/val/fatal-boot-processor-not-assigned-to-scheduler
 *
 * @ingroup TestsuitesFatalBootProcessorNotAssignedToScheduler
 *
 * @brief Tests a fatal error.
 *
 * This test case performs the following actions:
 *
 * - The test action is carried out by the application configuration of the
 *   test suite.
 *
 *   - Check that the expected fatal source is present.
 *
 *   - Check that the expected fatal code is present.
 *
 * @{
 */

/**
 * @brief Test context for
 *   spec:/score/smp/val/fatal-boot-processor-not-assigned-to-scheduler test
 *   case.
 */
typedef struct {
  /**
   * @brief This member contains a copy of the corresponding
   *   ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Run() parameter.
   */
  rtems_fatal_source source;

  /**
   * @brief This member contains a copy of the corresponding
   *   ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Run() parameter.
   */
  rtems_fatal_code code;
} ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Context;

static ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Context
  ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Instance;

static T_fixture ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Fixture = {
  .setup = NULL,
  .stop = NULL,
  .teardown = NULL,
  .scope = NULL,
  .initial_context = &ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Instance
};

/**
 * @brief The test action is carried out by the application configuration of
 *   the test suite.
 */
static void ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Action_0(
  ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Context *ctx
)
{
  /* Nothing to do */

  /*
   * Check that the expected fatal source is present.
   */
  T_step_eq_int( 0, ctx->source, RTEMS_FATAL_SOURCE_SMP );

  /*
   * Check that the expected fatal code is present.
   */
  T_step_eq_ulong(
    1,
    ctx->code,
    SMP_FATAL_BOOT_PROCESSOR_NOT_ASSIGNED_TO_SCHEDULER
  );
}

void ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Run(
  rtems_fatal_source source,
  rtems_fatal_code   code
)
{
  ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Context *ctx;

  ctx = &ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Instance;
  ctx->source = source;
  ctx->code = code;

  ctx = T_case_begin(
    "ScoreSmpValFatalBootProcessorNotAssignedToScheduler",
    &ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Fixture
  );

  T_plan( 2 );

  ScoreSmpValFatalBootProcessorNotAssignedToScheduler_Action_0( ctx );

  T_case_end();
}

/** @} */
