
 package test.net.sourceforge.pmd.jsp.rules;
 
 import net.sourceforge.pmd.Rule;

import org.junit.Before;
import org.junit.Test;

import test.net.sourceforge.pmd.testframework.SimpleAggregatorTst;
 
 public class NoLongScriptsTest extends SimpleAggregatorTst {
     private Rule rule;
 
     @Before
     public void setUp() {
         rule = findRule("jsp", "NoLongScripts");
     }
 
     @Test
     public void testAll() {
         runTests(rule);
     }

     public static junit.framework.Test suite() {
         return new junit.framework.JUnit4TestAdapter(NoLongScriptsTest.class);
     }
}