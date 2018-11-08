#include "edge_wrapper.h"
#include <string.h>
/* Really all of this file should be autogenerated, that will happen
   eventually. */

int edge_init(Keystone* enclave){

  enclave->registerOcallDispatch(incoming_call_dispatch);
  register_call(1, print_buffer_wrapper);
  register_call(2, print_value_wrapper);
}

// TODO: again, should be autogenerated
size_t pbw_data_len = 64;


// TODO: This should be autogenerated
void print_buffer_wrapper(void* shared_buffer, size_t shared_buffer_size)
{
  /* For now we assume the call struct is at the front of the shared
   * buffer. This will have to change to allow nested calls. */
  struct edge_call_t* edge_call = (struct edge_call_t*)shared_buffer;

  uintptr_t data_section;
  unsigned long ret_val;
  if(edge_call_get_ptr_from_offset((uintptr_t)shared_buffer, shared_buffer_size,
				     edge_call->call_arg_offset, pbw_data_len,
				     &data_section) != 0){
    // Need to raise some error somewhere, oh well
    edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
    return;
  }

  ret_val = print_buffer((char*)data_section);
  // We are done with the data section for args, use as return region
  // TODO safety check?
  memcpy((void*)data_section, &ret_val, sizeof(unsigned long));  
  edge_call->return_data.call_status = CALL_STATUS_OK;

  if(edge_call_get_offset_from_ptr((uintptr_t)shared_buffer, shared_buffer_size,
				   data_section, sizeof(unsigned long),
				   &(edge_call->return_data.call_ret_offset)) != 0){
    
    edge_call->return_data.call_status = CALL_STATUS_BAD_OFFSET;
  }

  return;
}

void print_value_wrapper(void* shared_buffer, size_t shared_buffer_size)
{
  /* For now we assume the call struct is at the front of the shared
   * buffer. This will have to change to allow nested calls. */
  struct edge_call_t* edge_call = (struct edge_call_t*)shared_buffer;

  uintptr_t data_section;
  unsigned long ret_val;
  if(edge_call_get_ptr_from_offset((uintptr_t)shared_buffer, shared_buffer_size,
				     edge_call->call_arg_offset, pbw_data_len,
				     &data_section) != 0){
    // Need to raise some error somewhere, oh well
    edge_call->return_data.call_status = CALL_STATUS_BAD_PTR;
    return;
  }

  print_value(*(unsigned long*)data_section);

  edge_call->return_data.call_status = CALL_STATUS_OK;

  return;
}


