package org.quickfix.field; 
import org.quickfix.StringField; 
import java.util.Date; 

public class MDEntryID extends StringField 
{ 

  public MDEntryID() 
  { 
    super(278);
  } 
  public MDEntryID(String data) 
  { 
    super(278, data);
  } 
} 
