export project_path=/home/users/liudy/workspace/dimes 
export mochi=/home/users/liudy/software/spack-develop/var/spack/environments/rtai/.spack-env/view/

#g++ --std=c++17 -I $project_path -I $project_path/common -I $project_path/common/thirdparty -L $project_path/build main.cpp -lplasma_client -o main

# g++ --std=c++17 -I $project_path -I $project_path/common -I $project_path/common/thirdparty -L $project_path/build create.cpp -lplasma_client -o main

# g++ --std=c++17 -I $project_path -I $project_path/common -I $project_path/common/thirdparty -L $project_path/build pthread.cpp -lpthread -lplasma_client -o t

#g++ --std=c++17 -I $project_path -I $project_path/common -I $project_path/common/thirdparty -L $project_path/build put.c -lplasma_client -o put



g++ --std=c++17 -I $project_path -I $project_path/common -I $project_path/common/thirdparty -L$project_path/build  -I${mochi}/include -L${mochi}/lib  rpc_create.cpp -lmargo -labt -lmercury -ldimes_client -o a.out 
