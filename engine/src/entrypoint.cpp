#include "stdafx.h"
#include "application.h"
#include "util/crashHandler.h"

int main(int argc, char** argv) {
	Diamond::CrashHandler::initialize();
	Diamond::GetApp()->initialize();
	return 0;
}