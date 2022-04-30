#include "stdafx.h"
#include "application.h"
#include "util/crashHandler.h"
#include "src/assert.h"
#include "util/log.h"

int main(int argc, char** argv) {
	Diamond::CrashHandler::initialize();
	Diamond::getApp()->initialize();
	return 0;
}
