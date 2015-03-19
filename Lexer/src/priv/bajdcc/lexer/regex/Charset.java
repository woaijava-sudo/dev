package priv.bajdcc.lexer.regex;

import java.util.ArrayList;

import priv.bajdcc.lexer.token.MetaType;

/**
 * �ַ���
 * 
 * @author bajdcc
 */
public class Charset implements IRegexComponent {

	/**
	 * �����ķ�Χ������Χ��
	 */
	public ArrayList<CharacterRange> m_arrPositiveBounds = new ArrayList<CharacterRange>();

	/**
	 * �Ƿ�ȡ��
	 */
	public boolean m_bReverse = false;

	@Override
	public void visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		visitor.visitEnd(this);
	}

	/**
	 * ��ӷ�Χ
	 * 
	 * @param begin
	 *            ����
	 * @param end
	 *            ����
	 */
	public boolean addRange(char begin, char end) {
		if (begin > end) {
			return false;
		}
		for (CharacterRange range : m_arrPositiveBounds) {
			if (begin <= range.m_chLowerBound && end >= range.m_chUpperBound)
				return false;
		}
		m_arrPositiveBounds.add(new CharacterRange(begin, end));
		return true;
	}

	/**
	 * ����ַ�
	 * 
	 * @param ch
	 *            �ַ�
	 */
	public boolean addChar(char ch) {
		return addRange(ch, ch);
	}
	
	/**
	 * ��ǰ�ַ����Ƿ����ָ���ַ�
	 * @param ch �ַ�
	 * @return ���ҽ��
	 */
	public boolean include(char ch) {
		for (CharacterRange range : m_arrPositiveBounds) {
			if (range.include(ch)) {
				return true;
			}
		}
		return false;
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		boolean comma = false;
		for (CharacterRange range : m_arrPositiveBounds) {
			if (comma)
				sb.append(MetaType.COMMA.getChar());
			sb.append(range);
			if (!comma)
				comma = true;
		}
		return sb.toString();
	}
}
