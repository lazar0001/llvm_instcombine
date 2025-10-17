add_llvm_library( LLVMInstCombineNew MODULE
  InstCombineNew.cpp


  PLUGIN_TOOL
  opt
  )
