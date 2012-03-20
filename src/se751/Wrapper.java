package se751;
import javax.swing.JComponent;


public class Wrapper<T> {
	
	protected T component; 
	
	public Wrapper(T t) {
		if (!(t instanceof JComponent)) {
			throw new IllegalArgumentException("wrapper can only work on JComponent");
		}
		component = t;
	}

}
