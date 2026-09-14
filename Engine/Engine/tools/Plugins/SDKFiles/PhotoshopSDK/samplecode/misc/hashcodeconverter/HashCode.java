import java.awt.*;
import java.applet.Applet;
import java.awt.TextField;
import java.awt.event.*;
import java.applet.Applet;

public class HashCode extends Applet implements ActionListener, ItemListener {
	
	TextField hashCodeText;
	TextField hashCodeTextField;

	TextField integerText;
	TextField integerTextField;

	public void init() {
		
		hashCodeText = new TextField();
		hashCodeText.setEditable(false);
		hashCodeText.setText(new String("Enter a 'Hash' Code Here:"));

		hashCodeTextField = new TextField();
		hashCodeTextField.setEditable(true);

		integerText = new TextField();
		integerText.setEditable(false);
		integerText.setText(new String("Enter an Integer Here:"));

		integerTextField = new TextField();
		integerTextField.setEditable(true);

        hashCodeTextField.addActionListener(this);
        integerTextField.addActionListener(this);

		setLayout(new java.awt.GridLayout(0,2));

		add(hashCodeText);
		add(hashCodeTextField);
		add(integerText);
		add(integerTextField);
	}

    public void itemStateChanged(ItemEvent e) {
    }

    public void actionPerformed(ActionEvent e) {
		
		Object source = e.getSource();
		String label = e.getActionCommand();
		
		if ( source == hashCodeTextField ) 
		{
			
			long i = label.charAt(1) * 256 * 256 * 256;
			i += label.charAt(2) * 256 * 256;
			i += label.charAt(3) * 256;
			i += label.charAt(4);

			String r = Long.toString(i);

			integerTextField.setText(r);
						
		} 
		else 
		{ 
			if ( source == integerTextField ) 
			{
			
				long i = Long.parseLong(label);

				long subtractor = 256 * 256 * 256;

				String r = new String();
				r = "'";

				short x = 1;

				while ( x < 5 )
				{
					char thisOne = (char)(i / subtractor);
					r += thisOne;
					i -= thisOne * subtractor;
					x++;
					subtractor /= 256;
				}

				r += "'";

				hashCodeTextField.setText(r);
			} 
		}
    }
}
