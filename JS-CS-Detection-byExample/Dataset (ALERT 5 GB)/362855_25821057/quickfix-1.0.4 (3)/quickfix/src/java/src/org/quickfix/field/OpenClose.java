package org.quickfix.field; 
import org.quickfix.CharField; 
import java.util.Date; 

public class OpenClose extends CharField 
{ 

  public OpenClose() 
  { 
    super(77);
  } 
  public OpenClose(char data) 
  { 
    super(77, data);
  } 
} 