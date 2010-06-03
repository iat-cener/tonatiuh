# Install script for directory: /home/manuel/Downloads/marble/data

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/credits_authors.html"
    "/home/manuel/Downloads/marble/data/credits_data.html"
    "/home/manuel/Downloads/marble/data/legend.html"
    "/home/manuel/Downloads/marble/data/legend.css"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/seacolors.leg"
    "/home/manuel/Downloads/marble/data/precipcolors.leg"
    "/home/manuel/Downloads/marble/data/tempcolors.leg"
    "/home/manuel/Downloads/marble/data/landcolors.leg"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/bitmaps" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/bitmaps/checkbox_checked.png"
    "/home/manuel/Downloads/marble/data/bitmaps/border_2.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_4_yellow.png"
    "/home/manuel/Downloads/marble/data/bitmaps/crater.png"
    "/home/manuel/Downloads/marble/data/bitmaps/pole_2.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_1_red.png"
    "/home/manuel/Downloads/marble/data/bitmaps/river.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_2_white.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_4_red.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_1_orange.png"
    "/home/manuel/Downloads/marble/data/bitmaps/osm.png"
    "/home/manuel/Downloads/marble/data/bitmaps/manned_landing.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_3_orange.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_1_yellow.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_4_white.png"
    "/home/manuel/Downloads/marble/data/bitmaps/wikipedia.png"
    "/home/manuel/Downloads/marble/data/bitmaps/volcano_1.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_2_orange.png"
    "/home/manuel/Downloads/marble/data/bitmaps/lake.png"
    "/home/manuel/Downloads/marble/data/bitmaps/observatory.png"
    "/home/manuel/Downloads/marble/data/bitmaps/default_location.png"
    "/home/manuel/Downloads/marble/data/bitmaps/checkbox_disabled.png"
    "/home/manuel/Downloads/marble/data/bitmaps/robotic_rover.png"
    "/home/manuel/Downloads/marble/data/bitmaps/nation.png"
    "/home/manuel/Downloads/marble/data/bitmaps/mountain_1.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_3_white.png"
    "/home/manuel/Downloads/marble/data/bitmaps/pole_1.png"
    "/home/manuel/Downloads/marble/data/bitmaps/coordinate.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_3_yellow.png"
    "/home/manuel/Downloads/marble/data/bitmaps/unmanned_soft_landing.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_1_white.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_3_red.png"
    "/home/manuel/Downloads/marble/data/bitmaps/ocean.png"
    "/home/manuel/Downloads/marble/data/bitmaps/shipwreck.png"
    "/home/manuel/Downloads/marble/data/bitmaps/airport.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_2_red.png"
    "/home/manuel/Downloads/marble/data/bitmaps/border_1.png"
    "/home/manuel/Downloads/marble/data/bitmaps/waypoint.png"
    "/home/manuel/Downloads/marble/data/bitmaps/checkbox_empty.png"
    "/home/manuel/Downloads/marble/data/bitmaps/valley.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_2_yellow.png"
    "/home/manuel/Downloads/marble/data/bitmaps/city_4_orange.png"
    "/home/manuel/Downloads/marble/data/bitmaps/other.png"
    "/home/manuel/Downloads/marble/data/bitmaps/unmanned_hard_landing.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/bitmaps" TYPE FILE FILES "/home/manuel/Downloads/marble/data/bitmaps/earth_apollo.jpg")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/bitmaps" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/bitmaps/cursor_bc.xpm"
    "/home/manuel/Downloads/marble/data/bitmaps/cursor_br.xpm"
    "/home/manuel/Downloads/marble/data/bitmaps/cursor_tl.xpm"
    "/home/manuel/Downloads/marble/data/bitmaps/cursor_bl.xpm"
    "/home/manuel/Downloads/marble/data/bitmaps/cursor_cl.xpm"
    "/home/manuel/Downloads/marble/data/bitmaps/cursor_cr.xpm"
    "/home/manuel/Downloads/marble/data/bitmaps/cursor_tr.xpm"
    "/home/manuel/Downloads/marble/data/bitmaps/cursor_tc.xpm"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/svg" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/svg/compass.svg"
    "/home/manuel/Downloads/marble/data/svg/marble-logo.svg"
    "/home/manuel/Downloads/marble/data/svg/marble-logo.png"
    "/home/manuel/Downloads/marble/data/svg/marble-logo-72dpi.png"
    "/home/manuel/Downloads/marble/data/svg/marble-logo-inverted-72dpi.png"
    "/home/manuel/Downloads/marble/data/svg/application-x-marble.svg"
    "/home/manuel/Downloads/marble/data/svg/application-x-marble.png"
    "/home/manuel/Downloads/marble/data/svg/application-x-marble-gray.png"
    "/home/manuel/Downloads/marble/data/svg/worldmap.svg"
    "/home/manuel/Downloads/marble/data/svg/lunarmap.svg"
    "/home/manuel/Downloads/marble/data/svg/coordinate.svg"
    "/home/manuel/Downloads/marble/data/svg/wikipedia.svg"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/flags" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/flags/flag_vn.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ua.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_si.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_vi.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_dj.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pg.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ms.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gl.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bt.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pn.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ht.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_uz.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gs.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tt.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_kn.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_jo.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sn.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ve.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ck.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_so.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_la.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_fm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_aw.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_er.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_zw.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gw.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pa.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_hm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gq.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ma.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_kh.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_az.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ar.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_at.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_fr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_eh.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ly.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_lc.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_jp.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ch.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bh.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gy.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_li.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_id.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gg.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_et.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_my.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mk.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_hr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cu.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tk.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ne.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_re.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_np.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_jm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mo.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sc.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_fi.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mu.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_zm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gb.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ee.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bi.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_md.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gd.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bs.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_by.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_na.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_vg.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_af.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tj.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_lu.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_lt.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bg.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_fo.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sb.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_kw.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_aq.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cx.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gn.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_es.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cl.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_us.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mz.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_im.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gp.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ge.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gh.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_yt.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_vu.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_nr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ax.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sv.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_am.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_nc.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_lk.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ir.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_be.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cn.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_au.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_is.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sz.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_dm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_nz.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ye.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tc.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ke.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_in.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mq.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_nf.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_hn.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pt.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ws.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_je.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ng.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ci.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bf.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_as.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_kp.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pk.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_me.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_rs.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_vc.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_no.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ba.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ao.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mp.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sh.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mc.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_to.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_nl.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_st.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_rw.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ls.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_do.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_uy.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ki.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ec.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cd.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ga.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ml.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tv.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_za.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_om.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tz.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mw.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cf.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mn.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gf.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gt.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_hk.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ae.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_kr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cz.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mm.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_dz.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_qa.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tn.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ug.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sd.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_py.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ru.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_kg.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_eg.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_nu.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_fk.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ie.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sy.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_td.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_de.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_lb.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_um.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bz.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pl.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cy.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_kz.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bv.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mg.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bd.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cv.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mt.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pw.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mv.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_lr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_wf.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_an.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mx.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sa.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pf.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tg.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bj.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ca.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_hu.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ps.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_pe.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ni.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cg.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sk.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tl.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sg.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_br.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bo.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bw.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gu.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ro.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ph.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_iq.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sj.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bn.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mh.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_km.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_lv.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ag.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_io.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_it.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_il.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_mr.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_th.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_gi.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_dk.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tw.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_al.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_co.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_cc.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_fj.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_se.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_bb.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ad.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_sl.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ai.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_ky.svg"
    "/home/manuel/Downloads/marble/data/flags/flag_tf.svg"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/citylights" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/citylights/citylights.jpg"
    "/home/manuel/Downloads/marble/data/maps/earth/citylights/citylights.dgml"
    "/home/manuel/Downloads/marble/data/maps/earth/citylights/citylights-preview.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/citylights/legend" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/citylights/legend/city.png"
    "/home/manuel/Downloads/marble/data/maps/earth/citylights/legend/lightterrain.png"
    "/home/manuel/Downloads/marble/data/maps/earth/citylights/legend/darkterrain.png"
    "/home/manuel/Downloads/marble/data/maps/earth/citylights/legend/water.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/srtm" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/srtm/srtm.dgml"
    "/home/manuel/Downloads/marble/data/maps/earth/srtm/srtm-preview.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/bluemarble" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/bluemarble/bluemarble.jpg"
    "/home/manuel/Downloads/marble/data/maps/earth/bluemarble/bluemarble.dgml"
    "/home/manuel/Downloads/marble/data/maps/earth/bluemarble/bluemarble-preview.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/bluemarble/legend" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/bluemarble/legend/ice.png"
    "/home/manuel/Downloads/marble/data/maps/earth/bluemarble/legend/mountain.png"
    "/home/manuel/Downloads/marble/data/maps/earth/bluemarble/legend/vegetation.png"
    "/home/manuel/Downloads/marble/data/maps/earth/bluemarble/legend/desert.png"
    "/home/manuel/Downloads/marble/data/maps/earth/bluemarble/legend/water.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/plain" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/plain/plain.dgml"
    "/home/manuel/Downloads/marble/data/maps/earth/plain/plain-preview.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/openstreetmap" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/openstreetmap.dgml"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/openstreetmap-preview.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend.html"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/openstreetmap/legend" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/airportapron.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/motorway.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/track.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/airportrunway.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/primaryroad.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/tram.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/boundary.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/railway.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/trunkroad.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/bridge.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/railwaystation.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/tunnel.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/bridleway.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/secondaryroad.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/unclassifiedroad.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/byway.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/significantbuilding.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/unsurfacedroad.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/cycleway.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/subway.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/footway.png"
    "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/legend/summitpeak.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/openstreetmap/0/0" TYPE FILE FILES "/home/manuel/Downloads/marble/data/maps/earth/openstreetmap/0/0/0.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/temp-july" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/temp-july/temp-july.jpg"
    "/home/manuel/Downloads/marble/data/maps/earth/temp-july/temp-july-preview.png"
    "/home/manuel/Downloads/marble/data/maps/earth/temp-july/temp-july.dgml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/temp-dec" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/temp-dec/temp-dec.jpg"
    "/home/manuel/Downloads/marble/data/maps/earth/temp-dec/temp-dec-preview.png"
    "/home/manuel/Downloads/marble/data/maps/earth/temp-dec/temp-dec.dgml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/precip-july" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/precip-july/precip-july.jpg"
    "/home/manuel/Downloads/marble/data/maps/earth/precip-july/precip-july-preview.png"
    "/home/manuel/Downloads/marble/data/maps/earth/precip-july/precip-july.dgml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/precip-dec" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/precip-dec/precip-dec.jpg"
    "/home/manuel/Downloads/marble/data/maps/earth/precip-dec/precip-dec-preview.png"
    "/home/manuel/Downloads/marble/data/maps/earth/precip-dec/precip-dec.dgml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/schagen1689" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/schagen1689/legend.html"
    "/home/manuel/Downloads/marble/data/maps/earth/schagen1689/schagen1689.jpg"
    "/home/manuel/Downloads/marble/data/maps/earth/schagen1689/schagen1689.dgml"
    "/home/manuel/Downloads/marble/data/maps/earth/schagen1689/schagen1689-preview.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/earth/schagen1689/legend" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/earth/schagen1689/legend/mountain.png"
    "/home/manuel/Downloads/marble/data/maps/earth/schagen1689/legend/town.png"
    "/home/manuel/Downloads/marble/data/maps/earth/schagen1689/legend/river.png"
    "/home/manuel/Downloads/marble/data/maps/earth/schagen1689/legend/lake.png"
    "/home/manuel/Downloads/marble/data/maps/earth/schagen1689/legend/schagen_title.png"
    "/home/manuel/Downloads/marble/data/maps/earth/schagen1689/legend/schagen_orig.png"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/maps/moon/clementine" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/maps/moon/clementine/clementine.jpg"
    "/home/manuel/Downloads/marble/data/maps/moon/clementine/clementine-preview.png"
    "/home/manuel/Downloads/marble/data/maps/moon/clementine/legend.html"
    "/home/manuel/Downloads/marble/data/maps/moon/clementine/clementine.dgml"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/mwdbii" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/mwdbii/DATELINE.PNT"
    "/home/manuel/Downloads/marble/data/mwdbii/PCOAST.PNT"
    "/home/manuel/Downloads/marble/data/mwdbii/PDIFFBORDER.PNT"
    "/home/manuel/Downloads/marble/data/mwdbii/PGLACIER.PNT"
    "/home/manuel/Downloads/marble/data/mwdbii/PISLAND.PNT"
    "/home/manuel/Downloads/marble/data/mwdbii/PLAKEISLAND.PNT"
    "/home/manuel/Downloads/marble/data/mwdbii/PLAKE.PNT"
    "/home/manuel/Downloads/marble/data/mwdbii/PUSA48.DIFF.PNT"
    "/home/manuel/Downloads/marble/data/mwdbii/RIVER.PNT"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/stars" TYPE FILE FILES "/home/manuel/Downloads/marble/data/stars/stars.dat")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/marble/data/placemarks" TYPE FILE FILES
    "/home/manuel/Downloads/marble/data/placemarks/moonterrain.cache"
    "/home/manuel/Downloads/marble/data/placemarks/boundaryplacemarks.cache"
    "/home/manuel/Downloads/marble/data/placemarks/otherplacemarks.cache"
    "/home/manuel/Downloads/marble/data/placemarks/elevplacemarks.cache"
    "/home/manuel/Downloads/marble/data/placemarks/moonlandingsites.cache"
    "/home/manuel/Downloads/marble/data/placemarks/baseplacemarks.cache"
    "/home/manuel/Downloads/marble/data/placemarks/cityplacemarks.cache"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/icons/hicolor/128x128/apps" TYPE FILE RENAME "marble.png" FILES "/home/manuel/Downloads/marble/data/icons/hi128-app-marble.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/icons/hicolor/64x64/apps" TYPE FILE RENAME "marble.png" FILES "/home/manuel/Downloads/marble/data/icons/hi64-app-marble.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/icons/hicolor/48x48/apps" TYPE FILE RENAME "marble.png" FILES "/home/manuel/Downloads/marble/data/icons/hi48-app-marble.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/icons/hicolor/32x32/apps" TYPE FILE RENAME "marble.png" FILES "/home/manuel/Downloads/marble/data/icons/hi32-app-marble.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/icons/hicolor/22x22/apps" TYPE FILE RENAME "marble.png" FILES "/home/manuel/Downloads/marble/data/icons/hi22-app-marble.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/share/icons/hicolor/16x16/apps" TYPE FILE RENAME "marble.png" FILES "/home/manuel/Downloads/marble/data/icons/hi16-app-marble.png")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

