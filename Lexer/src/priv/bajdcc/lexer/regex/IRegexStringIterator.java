package priv.bajdcc.lexer.regex;

import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.error.RegexException.RegexError;
import priv.bajdcc.lexer.token.MetaType;

/**
 * 字符串迭代器接口
 * @author bajdcc
 *
 */
public interface IRegexStringIterator {

	/**
	 * 抛出错误
	 * 
	 * @param error
	 *            错误类型
	 * @throws RegexException
	 */
	public abstract void err(RegexError error) throws RegexException;

	/**
	 * 处理下一个字符
	 */
	public abstract void next();

	/**
	 * 判断当前位置不是末尾
	 * 
	 * @return 当前字符是否有效
	 */
	public abstract boolean available();

	/**
	 * 前进一个字符（look forward）
	 * 
	 */
	public abstract void advance();

	/**
	 * 获得当前字符
	 * 
	 * @return 当前字符
	 */
	public abstract char current();

	/**
	 * 确认当前字符
	 * 
	 * @param meta
	 *            类型
	 * @param error
	 *            抛出的错误
	 * @throws RegexException
	 */
	public abstract void expect(MetaType meta, RegexError error)
			throws RegexException;
}