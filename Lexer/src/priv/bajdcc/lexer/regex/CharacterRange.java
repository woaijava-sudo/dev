package priv.bajdcc.lexer.regex;

/**
 * 字符范围
 * 
 * @author bajdcc
 */
public class CharacterRange {

	/**
	 * 下限（包含）
	 */
	public char chLowerBound = 0;

	/**
	 * 上限（包含）
	 */
	public char chUpperBound = 0;
	
	public CharacterRange() {
		
	}
	
	public CharacterRange(char lower, char upper) {
		chLowerBound = lower;
		chUpperBound = upper;
	}
}
