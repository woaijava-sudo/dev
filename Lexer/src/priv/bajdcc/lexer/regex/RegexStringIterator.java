package priv.bajdcc.lexer.regex;

import priv.bajdcc.lexer.error.RegexException;
import priv.bajdcc.lexer.error.RegexException.RegexError;
import priv.bajdcc.lexer.token.MetaType;

/**
 * 字符串迭代器，提供字节流解析功能
 * 
 * @author bajdcc
 *
 */
public class RegexStringIterator implements IRegexStringIterator {
	/**
	 * 存储字符串
	 */
	protected String m_strContext;

	/**
	 * 当前的分析信息
	 */
	protected RegexStringIteratorData m_Data = new RegexStringIteratorData();

	public RegexStringIterator(String strContext) {
		m_strContext = strContext;
	}

	/* （非 Javadoc）
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator#err(priv.bajdcc.lexer.error.RegexException.RegexError)
	 */
	@Override
	public void err(RegexError error) throws RegexException {
		throw new RegexException(error, m_Data.m_iIndex);
	}

	/* （非 Javadoc）
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator#next()
	 */
	@Override
	public void next() {
		if (available()) {
			advance();
		}
		translate();
	}

	/**
	 * 翻译当前字符
	 */
	protected void translate() {
		if (!available()) {
			m_Data.m_chCurrent = 0;
			m_Data.m_kMeta = MetaType.END;
			return;
		}
		m_Data.m_chCurrent = current();
		transform();
	}
	
	/**
	 * 分析字符类型
	 */
	protected void transform() {
		m_Data.m_kMeta = MetaType.CHARACTER;
	}

	/* （非 Javadoc）
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator1#available()
	 */
	@Override
	public boolean available() {
		return m_Data.m_iIndex < m_strContext.length();
	}

	/* （非 Javadoc）
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator1#advance()
	 */
	@Override
	public void advance() {
		m_Data.m_iIndex++;
	}

	/* （非 Javadoc）
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator1#current()
	 */
	@Override
	public char current() {
		return m_strContext.charAt(m_Data.m_iIndex);
	}

	/* （非 Javadoc）
	 * @see priv.bajdcc.lexer.regex.IRegexStringIterator1#expect(priv.bajdcc.lexer.token.TokenUtility.MetaType, priv.bajdcc.lexer.error.RegexException.RegexError)
	 */
	@Override
	public void expect(MetaType meta, RegexError error) throws RegexException {
		if (m_Data.m_kMeta == meta) {
			next();
		} else {
			err(error);
		}
	}
}

/**
 * 分析时使用的数据
 */
class RegexStringIteratorData {
	/**
	 * 当前处理的位置
	 */
	public int m_iIndex = 0;

	/**
	 * 字符
	 */
	public char m_chCurrent = 0;

	/**
	 * 字符类型
	 */
	public MetaType m_kMeta = MetaType.END;

	public RegexStringIteratorData() {

	}

	public RegexStringIteratorData(int index, char current, MetaType meta) {
		m_iIndex = index;
		m_chCurrent = current;
		m_kMeta = meta;
	}
}