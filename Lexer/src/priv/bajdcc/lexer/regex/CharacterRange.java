package priv.bajdcc.lexer.regex;

/**
 * �ַ���Χ
 * 
 * @author bajdcc
 */
public class CharacterRange {

	/**
	 * ���ޣ�������
	 */
	public char chLowerBound = 0;

	/**
	 * ���ޣ�������
	 */
	public char chUpperBound = 0;
	
	public CharacterRange() {
		
	}
	
	public CharacterRange(char lower, char upper) {
		chLowerBound = lower;
		chUpperBound = upper;
	}
}
