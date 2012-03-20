package se751;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;


@SuppressWarnings("serial")
public class MainPanel extends JPanel {

	private JButton button1;
	private JButton button2;
	private JButton button3;
	
	public MainPanel() {
		button1 = new JButton("button1");
		button1.addActionListener(new ActionListener() {
			private int i = 0;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				button1.setText(++i + "");
			}
			
		});
		this.add(button1);
		
		button2 = new JButton("button2");
		button2.addActionListener(new AsyncActionListener() {
			private int i = 0;
			@Override
			protected void asyncActionPerformed(ActionEvent e) {
				try {
					ButtonWrapper button = new ButtonWrapper(button2);
					Thread.sleep(1000);
					button.setText(++i + "");
				} catch (InterruptedException e1) {
					e1.printStackTrace();
				}
			}
			
		});
		this.add(button2);
		
		button3 = new JButton("button3");
		button3.addActionListener(new ActionListener() {
			private int i = 0;
			@Override
			public void actionPerformed(ActionEvent arg0) {
				try {
					Thread.sleep(1000);
					button3.setText(++i + "");
				} catch (InterruptedException e1) {
					e1.printStackTrace();
				}
			}
			
		});
		this.add(button3);
	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		JFrame frame = new JFrame("Main Window");
		frame.add(new MainPanel());
		frame.pack();
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		frame.setLocation(screenSize.width / 2, screenSize.height / 2);
		frame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent arg0) {
			    System.exit(0);
			  }
		});
		frame.setVisible(true);
	}

}
