package cn.njnu.edu.bajdcc.lexer.regex;

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

	@Override
	public void Visit(IRegexComponentVisitor visitor) {
		visitor.visitBegin(this);
		visitor.visitEnd(this);		
	}
}
