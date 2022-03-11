@ECHO on

SET bin_dir=bin
SET src_dir=src
SET tests_dir=tests
SET source_files=%src_dir%\371pass.cpp %src_dir%\wallet.cpp %src_dir%\category.cpp %src_dir%\item.cpp
SET main_file=%src_dir%\main.cpp
SET executable=%bin_dir%\371pass.exe

IF "%1"=="" GOTO compile

SET testStr=%1%
SET testStr=%testStr:~0,4%
IF %testStr%==test (
  SET source_files=%source_files% %tests_dir%\%1%.cpp
  SET main_file=%bin_dir%\catch.o
  SET executable=%bin_dir%\371pass-test.exe

  IF NOT EXIST %bin_dir%\catch.o (
     g++ --std=c++14 -c %src_dir%\lib_catch_main.cpp -o %bin_dir%\catch.o
  )
)

:compile
IF NOT EXIST %bin_dir% MKDIR %bin_dir%
IF EXIST %executable% DEL %executable%
g++ --std=c++14 -pedantic -Wall %source_files% %main_file% -o %executable%

:end
