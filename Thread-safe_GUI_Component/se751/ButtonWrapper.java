package se751;
import javax.swing.JButton;
import javax.swing.SwingUtilities;


public class ButtonWrapper extends Wrapper<JButton> {

	public ButtonWrapper(JButton t) {
		super(t);
	}

	public void setText(final String text) {
		SwingUtilities.invokeLater(new Runnable() {

			@Override
			public void run() {
				component.setText(text);
			}
			
		});
		
	}
}
