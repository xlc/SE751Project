package se751;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public abstract class AsyncActionListener implements ActionListener {

	private static ExecutorService executer = Executors.newCachedThreadPool();
	
	@Override
	public void actionPerformed(final ActionEvent e) {
		executer.submit(new Runnable() {

			@Override
			public void run() {
				asyncActionPerformed(e);
			}
			
		});
	}
	
	protected abstract void asyncActionPerformed(ActionEvent e);

}
