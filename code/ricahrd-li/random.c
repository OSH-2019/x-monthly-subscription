#include<linux/bpf.h>
#include "bpf_helper.h"

struct bpf_map_def SEC("maps") flag_map = {
    .type = BPF_MAP_TYPE_ARRAY,
    .key_size = sizeof(__u16),
    .value_size = sizeof(__u8),
    .max_entries = 512,
};

struct bpf_map_def SEC("maps") mean_map = {
    .type = BPF_MAP_TYPE_ARRAY,
    .key_size = sizeof(__u8),
    .value_size = sizeof(__u64),
    .max_entries = 64,
};

struct bpf_map_def SEC("maps") var_map = {
    .type = BPF_MAP_TYPE_ARRAY,
    .key_size = sizeof(__u8),
    .value_size = sizeof(HighAccuracy128),
    .max_entries = 64,
};

int main(){
    /* input data; */
    data_map datas;  

    flag_map flags= bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(__u16),sizeof(__u8), 512);  
    mean_map means= bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(__u8),sizeof(__u64), 64);
    var_map vars= bpf_create_map(BPF_MAP_TYPE_ARRAY, sizeof(__u8),sizeof(HighAccuracy128), 64);
    
    __u32 rand;

    for(unsigned int i=0;!(i>>6);i++){
        __u64 mean=0;
        HighAccuracy128 var=0; 
        
        for(unsigned int j=0;!(j>>7);){
            rand=bpf_get_prandom_u32();
            rand=((rand<<23)>>23);  // mod 512
            //若抽中，对flags相应位赋一个特殊值，如(i+1)
            if(bpf_map_lookup_elem(flags,rand)!=(i+1)){
                bpf_map_update_elem(flags,rand,i+1,BPF_EXIST);
                // mean
                mean+=bpf_map_lookup_elem(datas;rand);
                // var: 先算E(X^2)
                add_128_64mul64(var,bpf_map_lookup_elem(datas;rand),bpf_map_lookup_elem(datas;rand)); 
                j++;
            }
        }// for j 

        mean>>=7;
        rs_7_128(var);
        // var=E(X^2)-(EX)^2
        sub_128_64mul64(var,mean,mean);        
        bpf_map_update_elem(means,i,mean,BPF_EXIST);
        bpf_map_update_elem(vars,i,0,BPF_EXIST);  
    }//for  i
    
}
