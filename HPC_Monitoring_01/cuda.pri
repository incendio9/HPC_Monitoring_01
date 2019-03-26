{

    CUDA_BIN_PATH = /usr/local/cuda-10.1/bin
    CUDA_INC_PATH = /usr/local/cuda-10.1/include


    isEmpty(CU_DIR):CU_DIR = .
    isEmpty(QMAKE_CPP_MOD_CU):QMAKE_CPP_MOD_CU =
    isEmpty(QMAKE_EXT_CPP_CU):QMAKE_EXT_CPP_CU = .cu



    CUDA_INC += $$join(INCLUDEPATH,'" -I"','-I"','"')
    # cuda_compute_architecture setting
    for(_a, CUDA_COMPUTE_ARCH):{
        formatted_arch =$$join(_a,'',' -gencode arch=compute_',',code=sm_$$_a')
        CUDA_ARCH += $$formatted_arch
    }

    message($$CUDA_ARCH)

    ## correctly formats CUDA_DEFINES for nvcc
    for(_defines, CUDA_DEFINES):{
        formatted_defines += -D$$_defines
    }
    CUDA_DEFINES = $$formatted_defines

    QMAKE_CUFLAGS += $$QMAKE_CXXFLAGS
    DebugBuild:QMAKE_CUFLAGS += $$QMAKE_CXXFLAGS_DEBUG
    ReleaseBuild:QMAKE_CUFLAGS += $$QMAKE_CXXFLAGS_RELEASE
    QMAKE_CUFLAGS += $$QMAKE_CXXFLAGS_RTTI_ON $$QMAKE_CXXFLAGS_WARN_ON $$QMAKE_CXXFLAGS_STL_ON
    message(QMAKE_CUFLAGS: $$QMAKE_CUFLAGS)
    NVCC_FLAGS = -lnvml -lmyLib -lopengl

    QMAKE_CUEXTRAFLAGS += -c
    QMAKE_EXTRA_VARIABLES += QMAKE_CUEXTRAFLAGS

    cuda.name = Cuda ${QMAKE_FILE_IN}
    cuda.input = CUSOURCES

    cuda.commands = $$CUDA_DIR/bin/nvcc $$CUDA_DEFINES -m64 -g -arch=$$CUDA_ARCH -c $$NVCCFLAGS $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -c -o ${QMAKE_FILE_OUT}
    cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}_cuda.o
    silent:cuda.commands = @echo nvcc ${QMAKE_FILE_IN} && $$cuda.commands

    cuda.dependency_type = TYPE_C
   # cuda.depend_command = $$CUDA_DIR/bin/nvcc $$CUDA_DEFINES -g -M $$CUDA_INC $$NVCCFLAGS ${QMAKE_FILE_NAME}
    # Tell Qt that we want add more stuff to the Makefile
    QMAKE_EXTRA_COMPILERS += cuda


    build_pass|isEmpty(BUILDS):cuclean.depends = compiler_cu_clean
    else:cuclean.CONFIG += recursive
    QMAKE_EXTRA_TARGETS += cuclean

}
