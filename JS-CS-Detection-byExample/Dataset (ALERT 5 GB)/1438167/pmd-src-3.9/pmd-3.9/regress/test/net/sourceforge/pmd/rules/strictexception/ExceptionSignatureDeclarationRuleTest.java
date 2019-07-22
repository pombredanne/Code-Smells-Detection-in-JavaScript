
 /**
  * BSD-style license; for more info see http://pmd.sourceforge.net/license.html
  */
 package test.net.sourceforge.pmd.rules.strictexception;
 
 import net.sourceforge.pmd.Rule;
 import test.net.sourceforge.pmd.testframework.SimpleAggregatorTst;
 
 public class ExceptionSignatureDeclarationRuleTest extends SimpleAggregatorTst {
 
     private Rule rule;
 
     public void setUp() {
         rule = findRule("strictexception", "SignatureDeclareThrowsException");
     }
 
     public void testAll() {
         runTests(rule);
     }
 }