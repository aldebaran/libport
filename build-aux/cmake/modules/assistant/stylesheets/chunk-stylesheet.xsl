<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version='1.0'>

  <xsl:import href="/usr/share/xml/docbook/stylesheet/nwalsh/xhtml/chunk.xsl"/>

  <xsl:param name="use.id.as.filename">1</xsl:param>
  <xsl:param name="suppress.navigation" select="1"/>
  <xsl:param name="suppress.header.navigation" select="1"/>
  <xsl:template name="user.header.content">
<div id="my_header">
<table width="100%" summary="Navigation header">
  <tr>
    <td width="20%" align="left" valign="top">
      <img src="images/urbiLab.png" alt="urbiLab" />
    </td>
    <td width="60%" align="center" valign="top">
      <a href="index.html">Home</a>
    </td>
    <td width="20%" align="right" valign="top">
      <img src="images/gostai.png" alt="Gostai" />
    </td>
  </tr>
</table>
</div>
  </xsl:template>

  <xsl:include href="base-html-stylesheet.xsl"/>

</xsl:stylesheet>
