/***
 *             __         _
 *      _   _ / _| ___   (_)___
 *     | | | | |_ / _ \  | / __|
 *     | |_| |  _| (_) | | \__
 *      \__,_|_|  \___(_)/ |___/
 *                     |__/
 *
 *     Proudly developed by:
 *
 *     boemianrapsodi   @gmail.com
 *     b3by.in.th3.sky  @gmail.com
 *
 *
 *     GitHub URL:
 *
 *     https://github.com/organizations/ufojs
 *
 *   This project is completely open source, and can be
 *   found on GitHub. So, if you like it and would like
 *   to collaborate, just let us know. We're totally on
 *   this sh*t, and appreciate comments and suggestions
 *   as much as we do for bacon.
 *   Do you know what else do we like? Bacon.
 *
 */

#include <node.h>
#include "peerconnection_wrapper.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  PeerconnectionWrapper::Init(exports);
}

NODE_MODULE(rtc, InitAll)
