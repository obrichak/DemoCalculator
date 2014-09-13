package com.demo.calculator;

import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.text.Editable;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.Toast;

public class CalculatorActivity extends Activity implements View.OnClickListener {

    private EditText calcField;
    private Map<Integer, String> keyboard;

    private static final int INP_STATUS_OK = 1;
    private static final int INP_STATUS_WRONG_END = -1;
    private static final int INP_STATUS_WRONG_PARENTHESES = -2;
    private static final int INP_STATUS_WRONG = -3;
    private static final int MAX_KEYS = 17;
    private static final String INFINITY = "Infinity";
    private static final String NAN = "NaN";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_calculator);
        
        System.loadLibrary("DemoCalculator");

        initKeyboard();

        calcField = (EditText) findViewById(R.id.calcField);
        calcField.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                InputMethodManager imm = (InputMethodManager) getApplicationContext()
                        .getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(calcField.getWindowToken(), 0);
            }
        });
    }

    @Override
    public void onClick(View v) {
        int viewId = v.getId();

        if (keyboard.containsKey(viewId)) {
            addSymbol(viewId);
        } else {
            switch (viewId) {
                case R.id.btnRemoveSymbol:
                    removeSymbol();
                    break;
                case R.id.btnClearAll:
                    Editable currentText = calcField.getText();
                    currentText.clear();
                    break;
                case R.id.btnEqual:
                    process();
                    break;
            }
        }

    }

    private void initKeyboard() {

        keyboard = new HashMap<Integer, String>(MAX_KEYS);

        int[] numbers = new int[] { R.id.btnZero, R.id.btnOne, R.id.btnTwo,
                R.id.btnThree, R.id.btnFour, R.id.btnFive, R.id.btnSix,
                R.id.btnSeven, R.id.btnEight, R.id.btnNine };

        int cntOfNumbers = numbers.length;

        for (int i = 0; i < cntOfNumbers; i++) {
            keyboard.put(numbers[i], String.valueOf(i));
        }

        keyboard.put(R.id.btnPlus, "+");
        keyboard.put(R.id.btnMinus, "-");
        keyboard.put(R.id.btnMultiply, "*");
        keyboard.put(R.id.btnDivide, "/");
        keyboard.put(R.id.btnLeftBracket, "(");
        keyboard.put(R.id.btnRightBracket, ")");
        keyboard.put(R.id.btnPoint, ".");
    }

    private void addSymbol(int viewId) {
        String current = calcField.getText().toString();
        int cursorPosition = calcField.getSelectionEnd();
        if (cursorPosition != current.length()) {
            calcField
                    .setText(current
                            .substring(0, cursorPosition)
                            .concat(keyboard.get(viewId))
                            .concat(current.substring(cursorPosition,
                                    current.length())));
        } else {
            calcField.setText(current.concat(keyboard.get(viewId)));
        }
        calcField.setSelection(cursorPosition + 1);
    }

    private void removeSymbol() {
        String current = calcField.getText().toString();
        if (current.equals(""))
            return;
        int cursorPosition = calcField.getSelectionEnd();
        if (cursorPosition == 0) {
            return;
        } else if (cursorPosition != current.length()) {
            calcField.setText(current.substring(0, cursorPosition - 1)
                    + current.substring(cursorPosition, current.length()));
        } else {
            calcField.setText(current.substring(0, current.length() - 1));
        }
        calcField.setSelection(cursorPosition - 1);
    }

    private void process() {
        String input = calcField.getText().toString();
        String message = "";
        int check = checkInput(input);
        switch (check) {
            case INP_STATUS_OK:
                if (!evaluate(input)) {
                    message = getString(R.string.output_wrong);
                    break;
                } else return;
            case INP_STATUS_WRONG_END:
                message = getString(R.string.input_wrong_end);
                break;
            case INP_STATUS_WRONG_PARENTHESES:
                message = getString(R.string.input_wrong_parentheses);
                break;
            case INP_STATUS_WRONG:
                message = getString(R.string.input_wrong);
                break;
        }
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show();
    }

    private boolean evaluate(String input) {
        double result = makeCalculations(input);
        String resultString = String.valueOf(result);
        if (resultString.equals(INFINITY)
                || resultString.equals(NAN)) {
            return false;
        } else {
            calcField.setText(String.valueOf(result));
            calcField.setSelection(calcField.getText().length());
            return true;
        }
    }

    native public static int checkInput(String input);

    native public static double makeCalculations(String input);
}
