import org.quickfix.Message;
import org.quickfix.FieldNotFound;
import junit.framework.TestCase;
import java.util.Date;
import java.util.Calendar;
import java.util.TimeZone;

public class MessageTest extends TestCase {

    static {
        System.loadLibrary("quickfix_jni_debug");
    }

    public MessageTest(String name) {
    	super(name);
    }

    public void setUp() throws Exception {
    }

    public void tearDown() throws Exception {
    }

    public void testMessageSetGetString() {
        Message message = new Message();

        try {
            message.getString(5);
            assertTrue("exception not thrown", false);
        } catch(FieldNotFound e) {}

        message.setString(5, "string5");

        try {
            assertEquals("string5", message.getString(5));
        } catch(FieldNotFound e) { assertTrue("exception thrown", false); }
    }

    public void setGetBoolean() {
        Message message = new Message();

        try {
            message.getBoolean(7);
            assertTrue("exception not thrown", false);
        } catch(FieldNotFound e) {}

        message.setBoolean(7, true);

        try {
            assertEquals(true, message.getBoolean(7));
        } catch(FieldNotFound e) { assertTrue("exception thrown", false); }
    }

    public void testMessageSetGetChar() {
        Message message = new Message();

        try {
            message.getChar(12);
            assertTrue("exception not thrown", false);
        } catch(FieldNotFound e) {}

        message.setChar(12, 'a');

        try {
            assertEquals('a', message.getChar(12));
        } catch(FieldNotFound e) { assertTrue("exception thrown", false); }
    }

    public void testMessageSetGetInt() {
        Message message = new Message();

        try {
            message.getInt(56);
            assertTrue("exception not thrown", false);
        } catch(FieldNotFound e) {}

        message.setInt(56, 23);

        try {
            assertEquals(23, message.getInt(56));
        } catch(FieldNotFound e) { assertTrue("exception thrown", false); }
    }

    public void testMessageSetGetDouble() {
        Message message = new Message();

        try {
            message.getDouble(9812);
            assertTrue("exception not thrown", false);
        } catch(FieldNotFound e) {}

        message.setDouble(9812, 12.3443);

        try {
            assertEquals(new Double(12.3443), 
                         new Double(message.getDouble(9812)));
        } catch(FieldNotFound e) { assertTrue("exception thrown", false); }
    }

    public void testMessageSetGetUtcTimeStamp() {
        Message message = new Message();

        try {
            message.getUtcTimeStamp(8);
            assertTrue("exception not thrown", false);
        } catch(FieldNotFound e) {}

        TimeZone timezone = TimeZone.getTimeZone("GMT+0");
        Calendar calendar = Calendar.getInstance(timezone);
        calendar.set(2002, 8, 6, 12, 34, 56);
        calendar.set(Calendar.MILLISECOND, 0);

        Date time = calendar.getTime();
        message.setUtcTimeStamp(8, time);

        try {
            assertEquals(message.getUtcTimeStamp(8).getTime(), time.getTime());
        } catch(FieldNotFound e) { assertTrue("exception thrown", false); }
    }
}