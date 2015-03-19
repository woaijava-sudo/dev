package priv.bajdcc.lexer.regex;

import java.util.Comparator;

/**
 * �ַ���Χ
 * 
 * @author bajdcc
 */
public class CharacterRange {

	/**
	 * ���ޣ�������
	 */
	public char m_chLowerBound = 0;

	/**
	 * ���ޣ�������
	 */
	public char m_chUpperBound = 0;

	public CharacterRange() {

	}

	public CharacterRange(char lower, char upper) {
		m_chLowerBound = lower;
		m_chUpperBound = upper;
	}
	
	/**
	 * ��ǰ�����Ƿ�����ַ�
	 * @param ch �ַ�
	 * @return �ȽϽ��
	 */
	public boolean include(char ch) {
		return ch >= m_chLowerBound && ch <= m_chUpperBound;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		if (m_chLowerBound == m_chUpperBound) {
			sb.append(printChar(m_chLowerBound));
		} else {
			sb.append(printChar(m_chLowerBound) + "-"
					+ printChar(m_chUpperBound));
		}
		return sb.toString();
	}

	private static String printChar(char ch) {
		if (Character.isISOControl(ch)) {
			return String.format("[\\u%04x]", (int) ch);
		} else {
			return String.format("[\\u%04x,'%c']", (int) ch, ch);
		}
	}

}

class CharacterRangeComparator implements Comparator<CharacterRange> {

	@Override
	public int compare(CharacterRange o1, CharacterRange o2) {
		if (o1.m_chLowerBound < o2.m_chLowerBound) {
			return -1;
		}
		if (o1.m_chLowerBound == o2.m_chLowerBound) {
			if (o1.m_chUpperBound < o2.m_chUpperBound) {
				return -1;
			}
			if (o1.m_chUpperBound == o2.m_chUpperBound) {
				return 0;
			}
			return 1;
		}
		return 1;
	}
}