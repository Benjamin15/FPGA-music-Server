#pragma once

#include <memory>
#include <restbed>
#include <iostream>

#include "../manager/ManagerStatistics.h"
#include "../utils/utils.h"

void get_statistics(const std::shared_ptr< restbed::Session > session);