<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

  <xsl:import href="extra_files.xsl"/>

  <xsl:output method="xml" indent="yes" />

  <xsl:template match="/">


<QtHelpProject version="1.0">
  <namespace>com.gostai</namespace>
  <virtualFolder>doc</virtualFolder>
  <filterSection>
    <toc>

    <!--   select="preface|chapter|appendix|part|reference|refentry|colophon|article|bibliography|glossary|index|" -->
    <xsl:for-each select="book/chapter|book/appendix">
      <xsl:variable name="lvl1" select="@id"/>
      <section title="{title}" ref="{$base_dir}/{@id}.html">
      <xsl:for-each select="sect1">
        <xsl:choose>
          <xsl:when test="position() &gt; 1">
            <section title="{title}" ref="{$base_dir}/{@id}.html"></section>
          </xsl:when>
          <xsl:otherwise>
            <section title="{title}" ref="{$base_dir}/{$lvl1}.html#{@id}"></section>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>
      </section>
    </xsl:for-each>

    </toc>

    <!-- Generate the list of keywords by parsing chapterinfo/keywordset
      sections. -->
    <keywords>
    <xsl:for-each select="book/chapter|book/appendix">
      <xsl:variable name="filename" select="@id"/>
      <xsl:for-each select="chapterinfo/keywordset/keyword">
        <keyword name="{text()}" ref="{$base_dir}/{$filename}.html"/>
      </xsl:for-each>
    </xsl:for-each>
    </keywords>

    <!-- Generate the list of files to be included in the qhp -->
    <files>
    <xsl:for-each select="book/chapter|book/appendix">
      <file><xsl:value-of select="$base_dir"/><xsl:value-of select="@id"/>.html</file>
      <xsl:for-each select="sect1">
        <xsl:if test="position() &gt; 1">
          <file><xsl:value-of select="$base_dir"/><xsl:value-of select="@id"/>.html</file>
        </xsl:if>
      </xsl:for-each>
    </xsl:for-each>
    <xsl:value-of select="$extra_files" disable-output-escaping="yes"/>
    </files>
  </filterSection>
</QtHelpProject>

  </xsl:template>
</xsl:stylesheet>
