## 按逻辑来说的话，在流水线中 call和ret指令不是要切换cwp吗？切cwp应该是和读寄存器在同一周期/流水级

## 以切换的那一周期末尾一瞬间为界，让cache中除开重叠的部分全部失效

## 流水写入级逻辑：
if（写入地址在cache中 && 该地址未失效）{
  写入cache
}
else if （写入地址所在的块在cache中 && 该地址已经失效 ）{
  if（本周期该块要 cache到Wbuffer  ==  本周期有指令要写入这个块）{
    写入Wbuffer //且优先级大于cache到Wbuffer
  }
  else if（本周起该块不 cache到Wbuffer == 本周起没有指令写入这个块）{
    写入cache
  }
  
}
else if （写入地址不在cache中 && 该地址已经失效）{
  if（本周期cache要把要写入的块搬到GRF  == 本周期要写回）{
    写入 GRF  //且优先级大于Wbuffer到GRF
  }
  if（本周期cache不把要写入的块搬到GRF == 本周起不写回）{
    写入Wbuffer
  }
}
