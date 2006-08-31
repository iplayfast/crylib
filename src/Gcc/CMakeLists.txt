PROJECT(UnitTest)
SUBDIRS(Common)
INCLUDE_DIRECTORIES(Common Common/zlib)
ADD_DEFINITIONS(-DVALIDATING)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: None Debug Release
	RelWithDebInfo MinSizeRel."
	      FORCE)
	      ENDIF(NOT CMAKE_BUILD_TYPE)

ADD_EXECUTABLE(UnitTest UnitTest.cpp 
Common/BitArray.cpp
Common/ClassBuilder.cpp
Common/ClassContainer.cpp
Common/ClassException.cpp
Common/ClassFileStream.cpp
Common/ClassFooter.cpp
Common/ClassFunction.cpp
Common/ClassHeader.cpp
Common/ClassInstance.cpp
Common/ClassList.cpp
Common/ClassMemStream.cpp
Common/ClassProperty.cpp
Common/ClassStream.cpp
Common/ClassString.cpp
Common/CodeFactory.cpp
Common/CryArray.cpp
Common/CryBackProp.cpp
Common/CryFunctionDef.cpp
Common/CryFuzzy.cpp
Common/CryNN.cpp
Common/CryObject.cpp
Common/CryPattern.cpp
Common/CryPatterns.cpp
Common/CrySet.cpp
Common/CryXML.cpp
Common/DefaultBuild.cpp
Common/HeadImp.cpp
Common/HugeInt.cpp
Common/PrimInstance.cpp
Common/PrimeTab.cpp
Common/Utility.cpp
Common/bignumfactor.cpp
Common/HugeDouble.cpp
Common/zlib/adler32.c
Common/zlib/compress.c
Common/zlib/crc32.c
Common/zlib/deflate.c
Common/zlib/inffast.c
Common/zlib/inflate.c
Common/zlib/inftrees.c
Common/zlib/trees.c
Common/zlib/uncompr.c
Common/zlib/zutil.c)
