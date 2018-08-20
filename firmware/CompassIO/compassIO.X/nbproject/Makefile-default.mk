#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/compassIO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/compassIO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=sources/communication.c sources/configBits.c sources/delay.c sources/i2c.c sources/lib.c sources/main.c sources/servomotor.c sources/spi.c sources/uart.c sources/utils.c sources/flashmemory.c sources/mag.c sources/gps.c sources/move.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/sources/communication.o ${OBJECTDIR}/sources/configBits.o ${OBJECTDIR}/sources/delay.o ${OBJECTDIR}/sources/i2c.o ${OBJECTDIR}/sources/lib.o ${OBJECTDIR}/sources/main.o ${OBJECTDIR}/sources/servomotor.o ${OBJECTDIR}/sources/spi.o ${OBJECTDIR}/sources/uart.o ${OBJECTDIR}/sources/utils.o ${OBJECTDIR}/sources/flashmemory.o ${OBJECTDIR}/sources/mag.o ${OBJECTDIR}/sources/gps.o ${OBJECTDIR}/sources/move.o
POSSIBLE_DEPFILES=${OBJECTDIR}/sources/communication.o.d ${OBJECTDIR}/sources/configBits.o.d ${OBJECTDIR}/sources/delay.o.d ${OBJECTDIR}/sources/i2c.o.d ${OBJECTDIR}/sources/lib.o.d ${OBJECTDIR}/sources/main.o.d ${OBJECTDIR}/sources/servomotor.o.d ${OBJECTDIR}/sources/spi.o.d ${OBJECTDIR}/sources/uart.o.d ${OBJECTDIR}/sources/utils.o.d ${OBJECTDIR}/sources/flashmemory.o.d ${OBJECTDIR}/sources/mag.o.d ${OBJECTDIR}/sources/gps.o.d ${OBJECTDIR}/sources/move.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/sources/communication.o ${OBJECTDIR}/sources/configBits.o ${OBJECTDIR}/sources/delay.o ${OBJECTDIR}/sources/i2c.o ${OBJECTDIR}/sources/lib.o ${OBJECTDIR}/sources/main.o ${OBJECTDIR}/sources/servomotor.o ${OBJECTDIR}/sources/spi.o ${OBJECTDIR}/sources/uart.o ${OBJECTDIR}/sources/utils.o ${OBJECTDIR}/sources/flashmemory.o ${OBJECTDIR}/sources/mag.o ${OBJECTDIR}/sources/gps.o ${OBJECTDIR}/sources/move.o

# Source Files
SOURCEFILES=sources/communication.c sources/configBits.c sources/delay.c sources/i2c.c sources/lib.c sources/main.c sources/servomotor.c sources/spi.c sources/uart.c sources/utils.c sources/flashmemory.c sources/mag.c sources/gps.c sources/move.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/compassIO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX340F512H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/sources/communication.o: sources/communication.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/communication.o.d 
	@${RM} ${OBJECTDIR}/sources/communication.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/communication.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/communication.o.d" -o ${OBJECTDIR}/sources/communication.o sources/communication.c   
	
${OBJECTDIR}/sources/configBits.o: sources/configBits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/configBits.o.d 
	@${RM} ${OBJECTDIR}/sources/configBits.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/configBits.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/configBits.o.d" -o ${OBJECTDIR}/sources/configBits.o sources/configBits.c   
	
${OBJECTDIR}/sources/delay.o: sources/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/delay.o.d 
	@${RM} ${OBJECTDIR}/sources/delay.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/delay.o.d" -o ${OBJECTDIR}/sources/delay.o sources/delay.c   
	
${OBJECTDIR}/sources/i2c.o: sources/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/i2c.o.d 
	@${RM} ${OBJECTDIR}/sources/i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/i2c.o.d" -o ${OBJECTDIR}/sources/i2c.o sources/i2c.c   
	
${OBJECTDIR}/sources/lib.o: sources/lib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/lib.o.d 
	@${RM} ${OBJECTDIR}/sources/lib.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/lib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/lib.o.d" -o ${OBJECTDIR}/sources/lib.o sources/lib.c   
	
${OBJECTDIR}/sources/main.o: sources/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/main.o.d 
	@${RM} ${OBJECTDIR}/sources/main.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/main.o.d" -o ${OBJECTDIR}/sources/main.o sources/main.c   
	
${OBJECTDIR}/sources/servomotor.o: sources/servomotor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/servomotor.o.d 
	@${RM} ${OBJECTDIR}/sources/servomotor.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/servomotor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/servomotor.o.d" -o ${OBJECTDIR}/sources/servomotor.o sources/servomotor.c   
	
${OBJECTDIR}/sources/spi.o: sources/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/spi.o.d 
	@${RM} ${OBJECTDIR}/sources/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/spi.o.d" -o ${OBJECTDIR}/sources/spi.o sources/spi.c   
	
${OBJECTDIR}/sources/uart.o: sources/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/uart.o.d 
	@${RM} ${OBJECTDIR}/sources/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/uart.o.d" -o ${OBJECTDIR}/sources/uart.o sources/uart.c   
	
${OBJECTDIR}/sources/utils.o: sources/utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/utils.o.d 
	@${RM} ${OBJECTDIR}/sources/utils.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/utils.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/utils.o.d" -o ${OBJECTDIR}/sources/utils.o sources/utils.c   
	
${OBJECTDIR}/sources/flashmemory.o: sources/flashmemory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/flashmemory.o.d 
	@${RM} ${OBJECTDIR}/sources/flashmemory.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/flashmemory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/flashmemory.o.d" -o ${OBJECTDIR}/sources/flashmemory.o sources/flashmemory.c   
	
${OBJECTDIR}/sources/mag.o: sources/mag.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/mag.o.d 
	@${RM} ${OBJECTDIR}/sources/mag.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/mag.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/mag.o.d" -o ${OBJECTDIR}/sources/mag.o sources/mag.c   
	
${OBJECTDIR}/sources/gps.o: sources/gps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/gps.o.d 
	@${RM} ${OBJECTDIR}/sources/gps.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/gps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/gps.o.d" -o ${OBJECTDIR}/sources/gps.o sources/gps.c   
	
${OBJECTDIR}/sources/move.o: sources/move.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/move.o.d 
	@${RM} ${OBJECTDIR}/sources/move.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/move.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/move.o.d" -o ${OBJECTDIR}/sources/move.o sources/move.c   
	
else
${OBJECTDIR}/sources/communication.o: sources/communication.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/communication.o.d 
	@${RM} ${OBJECTDIR}/sources/communication.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/communication.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/communication.o.d" -o ${OBJECTDIR}/sources/communication.o sources/communication.c   
	
${OBJECTDIR}/sources/configBits.o: sources/configBits.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/configBits.o.d 
	@${RM} ${OBJECTDIR}/sources/configBits.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/configBits.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/configBits.o.d" -o ${OBJECTDIR}/sources/configBits.o sources/configBits.c   
	
${OBJECTDIR}/sources/delay.o: sources/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/delay.o.d 
	@${RM} ${OBJECTDIR}/sources/delay.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/delay.o.d" -o ${OBJECTDIR}/sources/delay.o sources/delay.c   
	
${OBJECTDIR}/sources/i2c.o: sources/i2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/i2c.o.d 
	@${RM} ${OBJECTDIR}/sources/i2c.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/i2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/i2c.o.d" -o ${OBJECTDIR}/sources/i2c.o sources/i2c.c   
	
${OBJECTDIR}/sources/lib.o: sources/lib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/lib.o.d 
	@${RM} ${OBJECTDIR}/sources/lib.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/lib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/lib.o.d" -o ${OBJECTDIR}/sources/lib.o sources/lib.c   
	
${OBJECTDIR}/sources/main.o: sources/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/main.o.d 
	@${RM} ${OBJECTDIR}/sources/main.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/main.o.d" -o ${OBJECTDIR}/sources/main.o sources/main.c   
	
${OBJECTDIR}/sources/servomotor.o: sources/servomotor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/servomotor.o.d 
	@${RM} ${OBJECTDIR}/sources/servomotor.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/servomotor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/servomotor.o.d" -o ${OBJECTDIR}/sources/servomotor.o sources/servomotor.c   
	
${OBJECTDIR}/sources/spi.o: sources/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/spi.o.d 
	@${RM} ${OBJECTDIR}/sources/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/spi.o.d" -o ${OBJECTDIR}/sources/spi.o sources/spi.c   
	
${OBJECTDIR}/sources/uart.o: sources/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/uart.o.d 
	@${RM} ${OBJECTDIR}/sources/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/uart.o.d" -o ${OBJECTDIR}/sources/uart.o sources/uart.c   
	
${OBJECTDIR}/sources/utils.o: sources/utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/utils.o.d 
	@${RM} ${OBJECTDIR}/sources/utils.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/utils.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/utils.o.d" -o ${OBJECTDIR}/sources/utils.o sources/utils.c   
	
${OBJECTDIR}/sources/flashmemory.o: sources/flashmemory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/flashmemory.o.d 
	@${RM} ${OBJECTDIR}/sources/flashmemory.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/flashmemory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/flashmemory.o.d" -o ${OBJECTDIR}/sources/flashmemory.o sources/flashmemory.c   
	
${OBJECTDIR}/sources/mag.o: sources/mag.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/mag.o.d 
	@${RM} ${OBJECTDIR}/sources/mag.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/mag.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/mag.o.d" -o ${OBJECTDIR}/sources/mag.o sources/mag.c   
	
${OBJECTDIR}/sources/gps.o: sources/gps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/gps.o.d 
	@${RM} ${OBJECTDIR}/sources/gps.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/gps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/gps.o.d" -o ${OBJECTDIR}/sources/gps.o sources/gps.c   
	
${OBJECTDIR}/sources/move.o: sources/move.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/sources" 
	@${RM} ${OBJECTDIR}/sources/move.o.d 
	@${RM} ${OBJECTDIR}/sources/move.o 
	@${FIXDEPS} "${OBJECTDIR}/sources/move.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/sources/move.o.d" -o ${OBJECTDIR}/sources/move.o sources/move.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/compassIO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/compassIO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/compassIO.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/compassIO.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/compassIO.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
