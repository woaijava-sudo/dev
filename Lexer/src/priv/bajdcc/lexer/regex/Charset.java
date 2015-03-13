package priv.bajdcc.lexer.regex;

import java.util.ArrayList;

/**
 * 字符集
 * 
 * @author bajdcc
 */
public class Charset implements IRegexComponent {
	/**
	 * 包含的范围（正范围）
	 */
	public ArrayList<CharacterRange> arrPositiveBounds = new ArrayList<CharacterRange>();

	/**
	 * 是否取反
	 */
	public boolean bReverse = false;

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
	public CharacterType kChar = CharacterType.NORMAL;

	@Override
	public void Visit(IRegexComponentVisitor visitor) {
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
	public void addRange(char begin, char end) {
		if (begin > end) {
			end = begin;
		}
		arrPositiveBounds.add(new CharacterRange(begin, end));
	}

	/**
	 * 添加字符
	 * 
	 * @param ch
	 *            字符
	 */
	public void addChar(char ch) {
		arrPositiveBounds.add(new CharacterRange(ch, ch));
	}
}
