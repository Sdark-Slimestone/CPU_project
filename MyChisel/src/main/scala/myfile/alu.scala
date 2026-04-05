package myfile
import chisel3._
import _root_.circt.stage.ChiselStage
import chisel3.util._

class alu extends Module{
  val io = IO(new Bundle{
    val sw0 = Input(UInt(4.W))  //a
    val sw1 = Input(UInt(4.W))  //b
    val led0 = Output(UInt(4.W))   //c
    val sw2 = Input(UInt(3.W))  //sel
    val led1 = Output(Bool())   //zero
    val led2 = Output(Bool()) //overflow
    val led3 = Output(Bool())  //carry
  })
    
    val alutable = VecInit(
      io.sw0 + io.sw1,  
      io.sw0 - io.sw1,  
      ~ io.sw0,  
      io.sw0 & io.sw1,   
      io.sw0 | io.sw1, 
      io.sw0 ^ io.sw1,  
      Mux((io.sw0.asSInt < io.sw1.asSInt), 1.U(4.W), 0.U(4.W)),
      Mux((io.sw0 === io.sw1), 1.U(4.W), 0.U(4.W)) 
    )

    io.led0 := alutable(io.sw2)
    val isntzero = io.led0.orR
    io.led1 := !isntzero

    val a5 = 0.U(1.W) ## io.sw0   
    val b5 = 0.U(1.W) ## io.sw1
    val add5 = a5 + b5
    val add_carry = add5(4)                
    val add_res4 = add5(3,0)              
    val add_overflow = (io.sw0(3) === io.sw1(3)) && (add_res4(3) =/= io.sw0(3))

    val sub5 = a5 + (0.U(1.W) ## (~io.sw1)) + 1.U
    val sub_carry = sub5(4)             
    val sub_res4 = sub5(3,0)
    val b_neg = (~io.sw1).asUInt + 1.U      
    val sub_overflow = (io.sw0(3) === b_neg(3)) && (sub_res4(3) =/= io.sw0(3))

    io.led3 := MuxLookup(io.sw2, false.B)(Seq(
      0.U -> add_carry,
      1.U -> sub_carry
    ))
    io.led2 := MuxLookup(io.sw2, false.B)(Seq(
      0.U -> add_overflow,
      1.U -> sub_overflow
    ))
}

object alu extends App {
  ChiselStage.emitSystemVerilogFile(
    new alu,
    firtoolOpts = Array("-disable-all-randomization", "-strip-debug-info", "-default-layer-specialization=enable")
  )
}