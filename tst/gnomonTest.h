#pragma once

#include <QtTest>

#define GNOMONTEST_MAIN(TestMain, TestObject)	        \
    int TestMain(int argc, char *argv[]) {		    \
        QApplication app(argc, argv);               \
        TestObject tc;                              \
        return QTest::qExec(&tc, argc, argv);		\
    }

#define GNOMONTEST_MAIN_NOGUI(TestMain, TestObject)	\
    int TestMain(int argc, char *argv[]) {			\
        QCoreApplication app(argc, argv);           \
        TestObject tc;                              \
        return QTest::qExec(&tc, argc, argv);       \
    }
