package priv.bajdcc.lexer.regex;

import java.util.ArrayList;

import priv.bajdcc.lexer.token.TokenUtility.MetaType;

/**
 * 字符集
 * 
 * @author bajdcc
 */
public class Charset implements IRegexComponent {

	/**
	 * 包含的范围（正范围）
	 */
	public ArrayList<CharacterRange> m_arrPositiveBounds = new ArrayList<CharacterRange>();

	/**
	 * 是否取反
	 */
	public boolean m_bReverse = false;

	/**
	 * 功能
	 */
	public enum CharacterType {
		/**
		 * 正常模式：启用字符范围
		 */
		NORMAL,
		/**
		 * 匹配行首
		 */
		BEGIN,
		/**
		 * 匹配行末
		 */
		END
	}

	/**
	 * 字符集类型（匹配字符，匹配首，匹配末）
	 */
	public CharacterType m_kChar = CharacterType.NORMAL;

	@Override
	public void visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		visitor.visitEnd(this);
	}

	/**
	 * 添加范围
	 * 
	 * @param begin
	 *            上限
	 * @param end
	 *            下限
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
	 * 添加字符
	 * 
	 * @param ch
	 *            字符
	 */
	public boolean addChar(char ch) {
		return addRange(ch, ch);
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
