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
	public char m_chLowerBound = 0;

	/**
	 * 上限（包含）
	 */
	public char m_chUpperBound = 0;

	public CharacterRange() {

	}

	public CharacterRange(char lower, char upper) {
		m_chLowerBound = lower;
		m_chUpperBound = upper;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		if (m_chLowerBound == m_chUpperBound) {
			sb.append(printChar(m_chLowerBound));
		} else {
			sb.append(printChar(m_chLowerBound) + "-" + printChar(m_chUpperBound));
		}
		return sb.toString();
	}
	
	private static String printChar(char ch){
		if (Character.isISOControl(ch)){
			return String.format("[\\u%04x]", (int)ch);
		} else {
			return String.format("[\\u%04x,'%c']", (int)ch, ch);
		}
	}
}
