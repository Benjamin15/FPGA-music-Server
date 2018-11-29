#pragma once

#include <string>
#include <mutex>
#include <thread>
#include <iostream>
#include <fstream>
#include <time.h>

void write_log(std::string content);
void init_log();
void close_log();

static std::ofstream mStream;
static std::mutex mMutex;
const std::string file_path = "metadata/log.txt";